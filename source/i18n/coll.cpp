/*
******************************************************************************
* Copyright (C) 1996-2001, International Business Machines Corporation and   *
* others. All Rights Reserved.                                               *
******************************************************************************
*/

/**
* File coll.cpp
*
* Created by: Helena Shih
*
* Modification History:
*
*  Date        Name        Description
*  2/5/97      aliu        Modified createDefault to load collation data from
*                          binary files when possible.  Added related methods
*                          createCollationFromFile, chopLocale, createPathName.
*  2/11/97     aliu        Added methods addToCache, findInCache, which implement
*                          a Collation cache.  Modified createDefault to look in
*                          cache first, and also to store newly created Collation
*                          objects in the cache.  Modified to not use gLocPath.
*  2/12/97     aliu        Modified to create objects from RuleBasedCollator cache.
*                          Moved cache out of Collation class.
*  2/13/97     aliu        Moved several methods out of this class and into
*                          RuleBasedCollator, with modifications.  Modified
*                          createDefault() to call new RuleBasedCollator(Locale&)
*                          constructor.  General clean up and documentation.
*  2/20/97     helena      Added clone, operator==, operator!=, operator=, and copy
*                          constructor.
* 05/06/97     helena      Added memory allocation error detection.
* 05/08/97     helena      Added createInstance().
*  6/20/97     helena      Java class name change.
* 04/23/99     stephen     Removed EDecompositionMode, merged with 
*                          Normalizer::EMode
* 11/23/9      srl         Inlining of some critical functions
* 01/29/01     synwee      Modified into a C++ wrapper calling C APIs (ucol.h)
*/

#include "unicode/utypes.h"

#if !UCONFIG_NO_COLLATION

#include "unicode/coll.h"
#include "unicode/tblcoll.h"
#include "cmemory.h"
#include "mutex.h"
#include "iculserv.h"

U_NAMESPACE_BEGIN

// ------------------------------------------
//
// Registration
//

//-------------------------------------------

UBool
CollatorFactory::visible(void) const {
  return TRUE;
}

//-------------------------------------------

UnicodeString& 
CollatorFactory::getDisplayName(const Locale& objectLocale, 
                                const Locale& displayLocale,
                                UnicodeString& result)
{
  return objectLocale.getDisplayName(displayLocale, result);
}

// -------------------------------------

class ICUCollatorFactory : public ICUResourceBundleFactory {

  virtual UObject* create(const ICUServiceKey& key, const ICUService* service, UErrorCode& status) const;
};

UObject*
ICUCollatorFactory::create(const ICUServiceKey& key, const ICUService* service, UErrorCode& status) const {
    if (handlesKey(key, status)) {
        const LocaleKey& lkey = (const LocaleKey&)key;
        Locale loc;
        // make sure the requested locale is correct
        // default LocaleFactory uses currentLocale since that's the one vetted by handlesKey
        // but for ICU rb resources we use the actual one since it will fallback again
        lkey.canonicalLocale(loc);

        return Collator::makeInstance(loc, status);
    }
    return NULL;
}

// -------------------------------------

class ICUCollatorService : public ICULocaleService {
    public:
    ICUCollatorService()
        : ICULocaleService("Collator")
    {
        UErrorCode status = U_ZERO_ERROR;
        registerFactory(new ICUCollatorFactory(), status);
    }

    virtual UObject* cloneInstance(UObject* instance) const {
        return ((Collator*)instance)->clone();
    }

    virtual UObject* handleDefault(const ICUServiceKey& key, UnicodeString* actualID, UErrorCode& status) const {
        LocaleKey& lkey = (LocaleKey&)key;
        Locale loc;
        lkey.canonicalLocale(loc);
        return Collator::makeInstance(loc, status);
    }

	virtual UObject* getKey(ICUServiceKey& key, UnicodeString* actualReturn, UErrorCode& status) const {
		UnicodeString ar;
		if (actualReturn == NULL) {
			actualReturn = &ar;
		}
		Collator* result = (Collator*)ICULocaleService::getKey(key, actualReturn, status);
		if (result) {
			const LocaleKey& lkey = (const LocaleKey&)key;
			Locale canonicalLocale;
			Locale currentLocale;

			result->setLocales(lkey.canonicalLocale(canonicalLocale), 
				LocaleUtility::initLocaleFromName(*actualReturn, currentLocale));
		}
		return result;
	}

    virtual UBool isDefault() const {
        return countFactories() == 1;
    }
};

// -------------------------------------

class ICUCollatorService;

static ICULocaleService* gService = NULL;

static UMTX gLock = 0;

static ICULocaleService* 
getService(void)
{
  Mutex mutex(&gLock);
  if (gService == NULL) {
    gService = new ICUCollatorService();
  }

  return gService;
}

// -------------------------------------

static UBool
hasService(void) 
{
  Mutex mutex(&gLock);
  return gService != NULL;
}

// -------------------------------------

UCollator* 
Collator::createUCollator(const char *loc,
					   UErrorCode *status)
{
	UCollator *result = 0;
	if (status && U_SUCCESS(*status) && hasService()) {
		Locale desiredLocale(loc);
		Collator *col = (Collator*)gService->get(desiredLocale, *status);
		if (col && col->getDynamicClassID() == RuleBasedCollator::getStaticClassID()) {
			RuleBasedCollator *rbc = (RuleBasedCollator *)col;
			if (!rbc->dataIsOwned) {
				result = ucol_safeClone(rbc->ucollator, NULL, NULL, status);
			} else {
				result = rbc->ucollator;
				rbc->ucollator = NULL; // to prevent free on delete
			}
		}
		delete col;
	}
	return result;
}

// Collator public methods -----------------------------------------------

Collator* Collator::createInstance(UErrorCode& success) 
{
  if (U_FAILURE(success))
    return 0;
  return createInstance(Locale::getDefault(), success);
}

Collator* Collator::createInstance(const Locale& desiredLocale,
                                   UErrorCode& status)
{
  if (U_FAILURE(status)) 
    return 0;

  if (hasService()) {
    return (Collator*)gService->get(desiredLocale, status);
  }
  return makeInstance(desiredLocale, status);
}


Collator* Collator::makeInstance(const Locale&  desiredLocale, 
                                         UErrorCode& status)
{
  // A bit of explanation is required here. Although in the current 
  // implementation
  // Collator::createInstance() is just turning around and calling 
  // RuleBasedCollator(Locale&), this will not necessarily always be the 
  // case. For example, suppose we modify this code to handle a 
  // non-table-based Collator, such as that for Thai. In this case, 
  // createInstance() will have to be modified to somehow determine this fact
  // (perhaps a field in the resource bundle). Then it can construct the 
  // non-table-based Collator in some other way, when it sees that it needs 
  // to.
  // The specific caution is this: RuleBasedCollator(Locale&) will ALWAYS 
  // return a valid collation object, if the system if functioning properly.  
  // The reason is that it will fall back, use the default locale, and even 
  // use the built-in default collation rules. THEREFORE, createInstance() 
  // should in general ONLY CALL RuleBasedCollator(Locale&) IF IT KNOWS IN 
  // ADVANCE that the given locale's collation is properly implemented as a 
  // RuleBasedCollator.
  // Currently, we don't do this...we always return a RuleBasedCollator, 
  // whether it is strictly correct to do so or not, without checking, because 
  // we currently have no way of checking.

  RuleBasedCollator* collation = new RuleBasedCollator(desiredLocale, 
                                                       status);
  /* test for NULL */
  if (collation == 0) {
      status = U_MEMORY_ALLOCATION_ERROR;
      return 0;
  }
  if (U_FAILURE(status))
  {
    delete collation;
    collation = 0;
  }
  return collation;
}

// !!! dlf the following is obsolete, ignore registration for this

Collator *
Collator::createInstance(const Locale &loc,
                         UVersionInfo version,
                         UErrorCode &status) {
  Collator *collator;
  UVersionInfo info;

  collator=new RuleBasedCollator(loc, status);
  /* test for NULL */
  if (collator == 0) {
      status = U_MEMORY_ALLOCATION_ERROR;
      return 0;
  }

  if(U_SUCCESS(status)) {
    collator->getVersion(info);
    if(0!=uprv_memcmp(version, info, sizeof(UVersionInfo))) {
      delete collator;
      status=U_MISSING_RESOURCE_ERROR;
      return 0;
    }
  }
  return collator;
}

// implement deprecated, previously abstract method
Collator::EComparisonResult Collator::compare(const UnicodeString& source, 
                                    const UnicodeString& target) const
{
	UErrorCode ec = U_ZERO_ERROR;
	return (Collator::EComparisonResult)compare(source, target, ec);
}

// implement deprecated, previously abstract method
Collator::EComparisonResult Collator::compare(const UnicodeString& source,
                                    const UnicodeString& target,
                                    int32_t length) const
{
	UErrorCode ec = U_ZERO_ERROR;
	return (Collator::EComparisonResult)compare(source, target, length, ec);
}

// implement deprecated, previously abstract method
Collator::EComparisonResult Collator::compare(const UChar* source, int32_t sourceLength,
                                    const UChar* target, int32_t targetLength) 
                                    const
{
	UErrorCode ec = U_ZERO_ERROR;
	return (Collator::EComparisonResult)compare(source, sourceLength, target, targetLength, ec);
}

UBool Collator::equals(const UnicodeString& source, 
                          const UnicodeString& target) const
{
  UErrorCode ec = U_ZERO_ERROR;
  return (compare(source, target, ec) == UCOL_EQUAL);
}

UBool Collator::greaterOrEqual(const UnicodeString& source, 
                                  const UnicodeString& target) const
{
  UErrorCode ec = U_ZERO_ERROR;
  return (compare(source, target, ec) != UCOL_LESS);
}

UBool Collator::greater(const UnicodeString& source, 
                           const UnicodeString& target) const
{
  UErrorCode ec = U_ZERO_ERROR;
  return (compare(source, target, ec) == UCOL_GREATER);
}

// this API  ignores registered collators, since it returns an
// array of indefinite lifetime
const Locale* Collator::getAvailableLocales(int32_t& count) 
{
  return Locale::getAvailableLocales(count);
}

UnicodeString& Collator::getDisplayName(const Locale& objectLocale,
                                           const Locale& displayLocale,
                                           UnicodeString& name)
{
  if (hasService()) {
    return gService->getDisplayName(objectLocale.getName(), name, displayLocale);
  }
  return objectLocale.getDisplayName(displayLocale, name);
}

UnicodeString& Collator::getDisplayName(const Locale& objectLocale,
                                           UnicodeString& name)
{   
  return getDisplayName(objectLocale, Locale::getDefault(), name);
}

/* This is useless information */
/*void Collator::getVersion(UVersionInfo versionInfo) const
{
  if (versionInfo!=NULL)
    uprv_memcpy(versionInfo, fVersion, U_MAX_VERSION_LENGTH);
}
*/

// UCollator protected constructor destructor ----------------------------

/**
* Default constructor.
* Constructor is different from the old default Collator constructor.
* The task for determing the default collation strength and normalization mode
* is left to the child class.
*/
Collator::Collator()
    : UObject()
{
}

/**
* Constructor.
* Empty constructor, does not handle the arguments.
* This constructor is done for backward compatibility with 1.7 and 1.8.
* The task for handling the argument collation strength and normalization 
* mode is left to the child class.
* @param collationStrength collation strength
* @param decompositionMode
* @deprecated 2.4 use the default constructor instead
*/
Collator::Collator(UCollationStrength, UNormalizationMode )
    : UObject()
{
}

Collator::~Collator()
{
}

Collator::Collator(const Collator &other)
    : UObject(other)
{
}

int32_t Collator::getBound(const uint8_t       *source,
        int32_t             sourceLength,
        UColBoundMode       boundType,
        uint32_t            noOfLevels,
        uint8_t             *result,
        int32_t             resultLength,
        UErrorCode          &status) {
  return ucol_getBound(source, sourceLength, boundType, noOfLevels, result, resultLength, &status);
}

void
Collator::setLocales(const Locale& requestedLocale, const Locale& validLocale) {
}

// -------------------------------------

URegistryKey
Collator::registerInstance(Collator* toAdopt, const Locale& locale, UErrorCode& status) 
{
    if (U_SUCCESS(status)) {
        return getService()->registerInstance(toAdopt, locale, status);
    }
    return NULL;
}

// -------------------------------------

class CFactory : public LocaleKeyFactory {
private:
    CollatorFactory* _delegate;
    Hashtable* _ids;

public:
    CFactory(CollatorFactory* delegate) 
        : LocaleKeyFactory(delegate->visible() ? VISIBLE : INVISIBLE)
        , _delegate(delegate)
        , _ids(NULL)
    {
        UErrorCode status = U_ZERO_ERROR;
        int32_t count = 0;
        _ids = new Hashtable(status);
        if (_ids) {
            const UnicodeString * const idlist = _delegate->getSupportedIDs(count, status);
            for (int i = 0; i < count; ++i) {
                _ids->put(idlist[i], (void*)this, status);
            }
        }
    }

    virtual ~CFactory()
    {
        delete _delegate;
        delete _ids;
    }

    virtual UObject* create(const ICUServiceKey& key, const ICUService* service, UErrorCode& status) const;

  protected:
    virtual const Hashtable* getSupportedIDs(UErrorCode& status) const
    {
        if (U_SUCCESS(status)) {
            return _ids;
        }
        return NULL;
    }

    virtual UnicodeString&
        getDisplayName(const UnicodeString& id, const Locale& locale, UnicodeString& result) const;
};

UObject* 
CFactory::create(const ICUServiceKey& key, const ICUService* service, UErrorCode& status) const
{
    if (handlesKey(key, status)) {
        const LocaleKey& lkey = (const LocaleKey&)key;
        Locale validLoc;
        lkey.currentLocale(validLoc);
		return _delegate->createCollator(validLoc);
		/*
        Locale requestedLoc;
        lkey.canonicalLocale(requestedLoc);

        Collator* result = _delegate->createCollator(validLoc);
        if (result) {
            result->setLocales(requestedLoc, validLoc);
        }
        return result;
		*/
    }
    return NULL;
}

UnicodeString&
CFactory::getDisplayName(const UnicodeString& id, const Locale& locale, UnicodeString& result) const 
{
    if ((_coverage & 0x1) == 0) {
        UErrorCode status = U_ZERO_ERROR;
        const Hashtable* ids = getSupportedIDs(status);
        if (ids && (ids->get(id) != NULL)) {
            Locale loc;
            LocaleUtility::initLocaleFromName(id, loc);
            return _delegate->getDisplayName(loc, locale, result);
        }
    }
    result.setToBogus();
    return result;
}

URegistryKey 
Collator::registerFactory(CollatorFactory* toAdopt, UErrorCode& status)
{
    if (U_SUCCESS(status)) {
        CFactory* f = new CFactory(toAdopt);
        if (f) {
            return getService()->registerFactory(f, status);
        }
        status = U_MEMORY_ALLOCATION_ERROR;
    }
    return NULL;
}

// -------------------------------------

UBool 
Collator::unregister(URegistryKey key, UErrorCode& status) 
{
  if (U_SUCCESS(status)) {
    if (hasService()) {
      return gService->unregister(key, status);
    }
    status = U_ILLEGAL_ARGUMENT_ERROR;
  }
  return FALSE;
}

// -------------------------------------

StringEnumeration* 
Collator::getAvailableLocales(void)
{
  return getService()->getAvailableLocales();
}

// UCollator private data members ----------------------------------------

/* This is useless information */
/*const UVersionInfo Collator::fVersion = {1, 1, 0, 0};*/

// -------------------------------------

U_NAMESPACE_END

// defined in ucln_cmn.h

/**
 * Release all static memory held by collator.
 */
U_CFUNC UBool collator_cleanup(void) {
  if (gService) {
    delete gService;
    gService = NULL;
  }
  umtx_destroy(&gLock);
  return TRUE;
}

#endif /* #if !UCONFIG_NO_COLLATION */

/* eof */
