/********************************************************************
 * COPYRIGHT:
 * Copyright (c) 2001-2003, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/
/********************************************************************************
*
* File custrtrn.C
*
* Modification History:
*        Name                     Description
*        Ram                      String transformations test
*********************************************************************************
*/
/****************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include "cintltst.h"
#include "unicode/utypes.h"
#include "cmemory.h"
#include "unicode/ustring.h"

void addUCharTransformTest(TestNode** root);

static void Test_UChar_UTF32_API(void);
static void Test_UChar_UTF8_API(void);
static void Test_UChar_WCHART_API(void);

void 
addUCharTransformTest(TestNode** root)
{
   addTest(root, &Test_UChar_UTF32_API, "custrtrn/Test_UChar_UTF32_API");
   addTest(root, &Test_UChar_UTF8_API, "custrtrn/Test_UChar_UTF8_API");
   addTest(root, &Test_UChar_WCHART_API,  "custrtrn/Test_UChar_WCHART_API");
}

static const uint32_t src32[]={
    0x00A8, 0x3003, 0x3005, 0x2015, 0xFF5E, 0x2016, 0x2026, 0x2018, 0x000D, 0x000A,
    0x2019, 0x201C, 0x201D, 0x3014, 0x3015, 0x3008, 0x3009, 0x300A, 0x000D, 0x000A,
    0x300B, 0x300C, 0x300D, 0x300E, 0x300F, 0x3016, 0x3017, 0x3010, 0x000D, 0x000A,
    0x3011, 0x00B1, 0x00D7, 0x00F7, 0x2236, 0x2227, 0x7FC1, 0x8956, 0x000D, 0x000A,
    0x9D2C, 0x9D0E, 0x9EC4, 0x5CA1, 0x6C96, 0x837B, 0x5104, 0x5C4B, 0x000D, 0x000A,
    0x61B6, 0x81C6, 0x6876, 0x7261, 0x4E59, 0x4FFA, 0x5378, 0x57F7, 0x000D, 0x000A,
    0x57F4, 0x57F9, 0x57FA, 0x57FC, 0x5800, 0x5802, 0x5805, 0x5806, 0x000D, 0x000A,
    0x580A, 0x581E, 0x6BB5, 0x6BB7, 0x6BBA, 0x6BBC, 0x9CE2, 0x977C, 0x000D, 0x000A,
    0x6BBF, 0x6BC1, 0x6BC5, 0x6BC6, 0x6BCB, 0x6BCD, 0x6BCF, 0x6BD2, 0x000D, 0x000A,
    0x6BD3, 0x6BD4, 0x6BD6, 0x6BD7, 0x6BD8, 0x6BDB, 0x6BEB, 0x6BEC, 0x000D, 0x000A,
    0x6C05, 0x6C08, 0x6C0F, 0x6C11, 0x6C13, 0x6C23, 0x6C34, 0x0041, 0x000D, 0x000A,
    0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x000D, 0x000A,
    0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x000D, 0x000A,
    0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x000D, 0x000A,
    0x005B, 0x9792, 0x9CCC, 0x9CCD, 0x9CCE, 0x9CCF, 0x9CD0, 0x9CD3, 0x000D, 0x000A,
    0x9CD4, 0x9CD5, 0x9CD7, 0x9CD8, 0x9CD9, 0x9CDC, 0x9CDD, 0x9CDF, 0x000D, 0x000A,
    0x9785, 0x9791, 0x00BD, 0x0390, 0x0385, 0x0386, 0x0388, 0x0389, 0x000D, 0x000A,
    0x038E, 0x038F, 0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x000D, 0x000A,
    0x0396, 0x0397, 0x0398, 0x0399, 0x039A, 0x038A, 0x038C, 0x039C, 0x000D, 0x000A,
    /* test non-BMP code points */
    0x0002A699,
    0x0002A69C, 0x0002A69D, 0x0002A69E, 0x0002A69F, 0x0002A6A0, 0x0002A6A5, 0x0002A6A6, 0x0002A6A7, 0x0002A6A8, 0x0002A6AB,
    0x0002A6AC, 0x0002A6AD, 0x0002A6AE, 0x0002A6AF, 0x0002A6B0, 0x0002A6B1, 0x0002A6B3, 0x0002A6B5, 0x0002A6B6, 0x0002A6B7,
    0x0002A6B8, 0x0002A6B9, 0x0002A6BA, 0x0002A6BB, 0x0002A6BC, 0x0002A6BD, 0x0002A6BE, 0x0002A6BF, 0x0002A6C0, 0x0002A6C1,
    0x0002A6C2, 0x0002A6C3, 0x0002A6C4, 0x0002A6C8, 0x0002A6CA, 0x0002A6CB, 0x0002A6CD, 0x0002A6CE, 0x0002A6CF, 0x0002A6D0,
    0x0002A6D1, 0x0002A6D2, 0x0002A6D3, 0x0002A6D4, 0x0002A6D5,

    0x4DB3, 0x4DB4, 0x4DB5, 0x4E00, 0x4E00, 0x4E01, 0x4E02, 0x4E03, 0x000D, 0x000A,
    0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x33E0, 0x33E6, 0x000D, 0x000A,
    0x4E05, 0x4E07, 0x4E04, 0x4E08, 0x4E08, 0x4E09, 0x4E0A, 0x4E0B, 0x000D, 0x000A,
    0x4E0C, 0x0021, 0x0022, 0x0023, 0x0024, 0xFF40, 0xFF41, 0xFF42, 0x000D, 0x000A,
    0xFF43, 0xFF44, 0xFF45, 0xFF46, 0xFF47, 0xFF48, 0xFF49, 0xFF4A, 0x000D, 0x000A,0x0000 
};

static const UChar src16[] = {
    0x00A8, 0x3003, 0x3005, 0x2015, 0xFF5E, 0x2016, 0x2026, 0x2018, 0x000D, 0x000A,
    0x2019, 0x201C, 0x201D, 0x3014, 0x3015, 0x3008, 0x3009, 0x300A, 0x000D, 0x000A,
    0x300B, 0x300C, 0x300D, 0x300E, 0x300F, 0x3016, 0x3017, 0x3010, 0x000D, 0x000A,
    0x3011, 0x00B1, 0x00D7, 0x00F7, 0x2236, 0x2227, 0x7FC1, 0x8956, 0x000D, 0x000A,
    0x9D2C, 0x9D0E, 0x9EC4, 0x5CA1, 0x6C96, 0x837B, 0x5104, 0x5C4B, 0x000D, 0x000A,
    0x61B6, 0x81C6, 0x6876, 0x7261, 0x4E59, 0x4FFA, 0x5378, 0x57F7, 0x000D, 0x000A,
    0x57F4, 0x57F9, 0x57FA, 0x57FC, 0x5800, 0x5802, 0x5805, 0x5806, 0x000D, 0x000A,
    0x580A, 0x581E, 0x6BB5, 0x6BB7, 0x6BBA, 0x6BBC, 0x9CE2, 0x977C, 0x000D, 0x000A,
    0x6BBF, 0x6BC1, 0x6BC5, 0x6BC6, 0x6BCB, 0x6BCD, 0x6BCF, 0x6BD2, 0x000D, 0x000A,
    0x6BD3, 0x6BD4, 0x6BD6, 0x6BD7, 0x6BD8, 0x6BDB, 0x6BEB, 0x6BEC, 0x000D, 0x000A,
    0x6C05, 0x6C08, 0x6C0F, 0x6C11, 0x6C13, 0x6C23, 0x6C34, 0x0041, 0x000D, 0x000A,
    0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x000D, 0x000A,
    0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x000D, 0x000A,
    0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x000D, 0x000A,
    0x005B, 0x9792, 0x9CCC, 0x9CCD, 0x9CCE, 0x9CCF, 0x9CD0, 0x9CD3, 0x000D, 0x000A,
    0x9CD4, 0x9CD5, 0x9CD7, 0x9CD8, 0x9CD9, 0x9CDC, 0x9CDD, 0x9CDF, 0x000D, 0x000A,
    0x9785, 0x9791, 0x00BD, 0x0390, 0x0385, 0x0386, 0x0388, 0x0389, 0x000D, 0x000A,
    0x038E, 0x038F, 0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x000D, 0x000A,
    0x0396, 0x0397, 0x0398, 0x0399, 0x039A, 0x038A, 0x038C, 0x039C, 0x000D, 0x000A,
    
    /* test non-BMP code points */ 
    0xD869, 0xDE99, 0xD869, 0xDE9C, 0xD869, 0xDE9D, 0xD869, 0xDE9E, 0xD869, 0xDE9F, 
    0xD869, 0xDEA0, 0xD869, 0xDEA5, 0xD869, 0xDEA6, 0xD869, 0xDEA7, 0xD869, 0xDEA8, 
    0xD869, 0xDEAB, 0xD869, 0xDEAC, 0xD869, 0xDEAD, 0xD869, 0xDEAE, 0xD869, 0xDEAF,
    0xD869, 0xDEB0, 0xD869, 0xDEB1, 0xD869, 0xDEB3, 0xD869, 0xDEB5, 0xD869, 0xDEB6, 
    0xD869, 0xDEB7, 0xD869, 0xDEB8, 0xD869, 0xDEB9, 0xD869, 0xDEBA, 0xD869, 0xDEBB, 
    0xD869, 0xDEBC, 0xD869, 0xDEBD, 0xD869, 0xDEBE, 0xD869, 0xDEBF, 0xD869, 0xDEC0, 
    0xD869, 0xDEC1, 0xD869, 0xDEC2, 0xD869, 0xDEC3, 0xD869, 0xDEC4, 0xD869, 0xDEC8, 
    0xD869, 0xDECA, 0xD869, 0xDECB, 0xD869, 0xDECD, 0xD869, 0xDECE, 0xD869, 0xDECF, 
    0xD869, 0xDED0, 0xD869, 0xDED1, 0xD869, 0xDED2, 0xD869, 0xDED3, 0xD869, 0xDED4, 
    0xD869, 0xDED5,  

    0x4DB3, 0x4DB4, 0x4DB5, 0x4E00, 0x4E00, 0x4E01, 0x4E02, 0x4E03, 0x000D, 0x000A,
    0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x33E0, 0x33E6, 0x000D, 0x000A,
    0x4E05, 0x4E07, 0x4E04, 0x4E08, 0x4E08, 0x4E09, 0x4E0A, 0x4E0B, 0x000D, 0x000A,
    0x4E0C, 0x0021, 0x0022, 0x0023, 0x0024, 0xFF40, 0xFF41, 0xFF42, 0x000D, 0x000A,
    0xFF43, 0xFF44, 0xFF45, 0xFF46, 0xFF47, 0xFF48, 0xFF49, 0xFF4A, 0x000D, 0x000A,0x0000 
};


static void Test_UChar_UTF32_API(void){

    UErrorCode err = U_ZERO_ERROR;
    UChar uTemp[1];
    uint32_t u32Temp[1];
    UChar* uTarget=uTemp;
    const uint32_t* u32Src = src32;
    int32_t u32SrcLen = sizeof(src32)/4;
    int32_t uTargetLength = 0;
    int32_t uDestLen=0;
    const UChar* uSrc = src16;
    int32_t uSrcLen   = sizeof(src16)/2;
    uint32_t* u32Target = u32Temp;
    uint32_t u32TargetLength =0;
    int32_t u32DestLen =0;
    UBool failed = FALSE;
    int i= 0;
    {
        /* preflight */
        u_strToUTF32(u32Target,u32TargetLength, &u32DestLen, uSrc, uSrcLen,&err);
        if(err = U_BUFFER_OVERFLOW_ERROR){
            err = U_ZERO_ERROR; 
            u32Target = (uint32_t*) malloc (sizeof(uint32_t) * (u32DestLen+1));
            u32TargetLength = u32DestLen+1;
        
            u_strToUTF32(u32Target,u32TargetLength, &u32DestLen, uSrc, uSrcLen,&err);
        }
        else {
            log_err("Should have gotten U_BUFFER_OVERFLOW_ERROR");
        }
        failed = FALSE;
        /*for(i=0; i< u32DestLen; i++){
            printf("0x%08X, ",uTarget[i]);
            if(i%10==0){
                printf("\n");
            }
        }*/
        for(i=0; i< u32SrcLen; i++){
            if(u32Target[i] != src32[i]){
                log_verbose("u_strToUTF32() failed expected: \\U%08X got: \\U%08X \n", src32[i], u32Target[i]);
                failed =TRUE;
            }
        }
        if(failed){
            log_err("u_strToUTF32() failed \n");
        }

        /* preflight */
        u_strFromUTF32(uTarget,uTargetLength,&uDestLen,u32Src,u32SrcLen,&err);
        if(err = U_BUFFER_OVERFLOW_ERROR){
            err = U_ZERO_ERROR; 
            uTarget = (UChar*) malloc( sizeof(UChar) * (uDestLen+1));
            uTargetLength =  uDestLen+1;
            u_strFromUTF32(uTarget,uTargetLength,&uDestLen,u32Src,u32SrcLen,&err);

        }
        /*for(i=0; i< uDestLen; i++){
            printf("0x%04X, ",uTarget[i]);
            if(i%10==0){
                printf("\n");
            }
        }*/
        
        for(i=0; i< uDestLen; i++){
            if(uTarget[i] != src16[i]){
                log_verbose("u_strFromUTF32() failed expected: \\U%08X got: \\U%08X \n", src16[i] ,uTarget[i]);
                failed =TRUE;
            }
        }
        if(failed){
            log_err("u_strToUTF32() failed \n");
        }

        free(u32Target);
        free(uTarget);
    }
    {
        u32SrcLen = -1;
        uTargetLength = 0;
        uSrcLen =-1;
        u32TargetLength=0;
        failed = FALSE;

       /* preflight */
        u_strToUTF32(NULL,u32TargetLength, &u32DestLen, uSrc, uSrcLen,&err);
        if(err = U_BUFFER_OVERFLOW_ERROR){
            err = U_ZERO_ERROR; 
            u32Target = (uint32_t*) malloc (sizeof(uint32_t) * (u32DestLen+1));
            u32TargetLength = u32DestLen+1;
        
            u_strToUTF32(u32Target,u32TargetLength, &u32DestLen, uSrc, uSrcLen,&err);
        }
        else {
            log_err("Should have gotten U_BUFFER_OVERFLOW_ERROR");
        }
        failed = FALSE;

        for(i=0; i< u32SrcLen; i++){
            if(u32Target[i] != src32[i]){
                log_verbose("u_strToUTF32() failed expected: \\U%08X got: \\U%08X \n", src32[i], u32Target[i]);
                failed =TRUE;
            }
        }
        if(failed){
            log_err("u_strToUTF32() failed \n");
        }

        /* preflight */
        u_strFromUTF32(NULL,uTargetLength,&uDestLen,u32Src,u32SrcLen,&err);
        if(err = U_BUFFER_OVERFLOW_ERROR){
            err = U_ZERO_ERROR; 
            uTarget = (UChar*) malloc( sizeof(UChar) * (uDestLen+1));
            uTargetLength =  uDestLen+1;
            u_strFromUTF32(uTarget,uTargetLength,&uDestLen,u32Src,u32SrcLen,&err);

        }
        
        for(i=0; i< uDestLen; i++){
            if(uTarget[i] != src16[i]){
                log_verbose("u_strFromUTF32() failed expected: \\U%08X got: \\U%08X \n", src16[i] ,uTarget[i]);
                failed =TRUE;
            }
        }
        if(failed){
            log_err("u_strToUTF32() failed \n");
        }

        free(u32Target);
        free(uTarget);
    }
}


static void Test_UChar_UTF8_API(void){

    UErrorCode err = U_ZERO_ERROR;
    UChar uTemp[1];
    uint8_t u8Temp[1];
    UChar* uTarget=uTemp;
    const uint8_t* u8Src;
    int32_t u8SrcLen = 0;
    int32_t uTargetLength = 0;
    int32_t uDestLen=0;
    const UChar* uSrc = src16;
    int32_t uSrcLen   = sizeof(src16)/2;
    uint8_t* u8Target = u8Temp;
    int32_t u8TargetLength =0;
    int32_t u8DestLen =0;
    UBool failed = FALSE;
    int i= 0;
    {
        /* preflight */
        u_strToUTF8(u8Target,u8TargetLength, &u8DestLen, uSrc, uSrcLen,&err);
        if(err == U_BUFFER_OVERFLOW_ERROR){
            err = U_ZERO_ERROR;       
            u8Target = (uint8_t*) malloc (sizeof(uint8_t) * (u8DestLen+1));
            u8TargetLength = u8DestLen;
        
            u_strToUTF8(u8Target,u8TargetLength, &u8DestLen, uSrc, uSrcLen,&err);

        }
        else {
            log_err("Should have gotten U_BUFFER_OVERFLOW_ERROR");
        }
        failed = FALSE;
        /*for(i=0; i< u8DestLen; i++){
            printf("0x%04X, ",u8Target[i]);
            if(i%10==0){
                printf("\n");
            }
        }*/
        /*for(i=0; i< u8DestLen; i++){
            if(u8Target[i] != src8[i]){
                log_verbose("u_strToUTF8() failed expected: \\U%08X got: \\U%08X \n", src8[i], u8Target[i]);
                failed =TRUE;
            }
        }
        if(failed){
            log_err("u_strToUTF8() failed \n");
        }*/
        u8Src = u8Target;
        u8SrcLen = u8DestLen;

        /* preflight */
        u_strFromUTF8(uTarget,uTargetLength,&uDestLen,u8Src,u8SrcLen,&err);
        if(err == U_BUFFER_OVERFLOW_ERROR){
            err = U_ZERO_ERROR;
            uTarget = (UChar*) malloc( sizeof(UChar) * (uDestLen+1));
            uTargetLength =  uDestLen;

            u_strFromUTF8(uTarget,uTargetLength,&uDestLen,u8Src,u8SrcLen,&err);
        }
        else {
            log_err("Should have gotten U_BUFFER_OVERFLOW_ERROR");
        }
        /*for(i=0; i< uDestLen; i++){
            printf("0x%04X, ",uTarget[i]);
            if(i%10==0){
                printf("\n");
            }
        }*/
        
        for(i=0; i< uSrcLen; i++){
            if(uTarget[i] != src16[i]){
                log_verbose("u_strFromUTF8() failed expected: \\u%04X got: \\u%04X at index: %i \n", src16[i] ,uTarget[i],i);
                failed =TRUE;
            }
        }
        if(failed){
            log_err("u_strToUTF8() failed \n");
        }

        free(u8Target);
        free(uTarget);
    }
    {
        u8SrcLen = -1;
        uTargetLength = 0;
        uSrcLen =-1;
        u8TargetLength=0;
        failed = FALSE;
        /* preflight */
        u_strToUTF8(NULL,u8TargetLength, &u8DestLen, uSrc, uSrcLen,&err);
        if(err == U_BUFFER_OVERFLOW_ERROR){
            err = U_ZERO_ERROR;       
            u8Target = (uint8_t*) malloc (sizeof(uint8_t) * (u8DestLen+1));
            u8TargetLength = u8DestLen;
        
            u_strToUTF8(u8Target,u8TargetLength, &u8DestLen, uSrc, uSrcLen,&err);

        }
        else {
            log_err("Should have gotten U_BUFFER_OVERFLOW_ERROR");
        }
        failed = FALSE;
        /*for(i=0; i< u8DestLen; i++){
            printf("0x%04X, ",u8Target[i]);
            if(i%10==0){
                printf("\n");
            }
        }*/
        /*for(i=0; i< u8DestLen; i++){
            if(u8Target[i] != src8[i]){
                log_verbose("u_strToUTF8() failed expected: \\U%08X got: \\U%08X \n", src8[i], u8Target[i]);
                failed =TRUE;
            }
        }
        if(failed){
            log_err("u_strToUTF8() failed \n");
        }*/
        u8Src = u8Target;
        u8SrcLen = u8DestLen;

        /* preflight */
        u_strFromUTF8(NULL,uTargetLength,&uDestLen,u8Src,u8SrcLen,&err);
        if(err == U_BUFFER_OVERFLOW_ERROR){
            err = U_ZERO_ERROR;
            uTarget = (UChar*) malloc( sizeof(UChar) * (uDestLen+1));
            uTargetLength =  uDestLen;

            u_strFromUTF8(uTarget,uTargetLength,&uDestLen,u8Src,u8SrcLen,&err);
        }
        else {
            log_err("Should have gotten U_BUFFER_OVERFLOW_ERROR");
        }
        /*for(i=0; i< uDestLen; i++){
            printf("0x%04X, ",uTarget[i]);
            if(i%10==0){
                printf("\n");
            }
        }*/
        
        for(i=0; i< uSrcLen; i++){
            if(uTarget[i] != src16[i]){
                log_verbose("u_strFromUTF8() failed expected: \\u%04X got: \\u%04X at index: %i \n", src16[i] ,uTarget[i],i);
                failed =TRUE;
            }
        }
        if(failed){
            log_err("u_strToUTF8() failed \n");
        }

        free(u8Target);
        free(uTarget);
    }
}
static const uint16_t src16j[] = {
    0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x000D, 0x000A,
    0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x000D, 0x000A,
    0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x000D, 0x000A,
    0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x000D, 0x000A,
    0x0000,
    /* Test only ASCII */
    /*
    0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD,
    0x00AE, 0x00AF, 0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
    0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF, 0x00C0, 0x00C1,
    0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB,
    0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 
    0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF, 
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9,
    0x0054, 0x0000
    */
};
static const uint16_t src16WithNulls[] = {
    0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0000,
    0x0048, 0x0049, 0x004A, 0x000D, 0x000A, 0x0000,
    0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0000,
    0x0050, 0x0051, 0x0052, 0x000D, 0x000A, 0x0000,
    0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0000,
    0x0058, 0x0059, 0x005A, 0x000D, 0x000A, 0x0000,
    0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0000,
    0x0058, 0x0059, 0x005A, 0x000D, 0x000A, 0x0000,
    /* test only ASCII */
    /* 
    0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD,
    0x00AE, 0x00AF, 0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7,
    0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF, 0x00C0, 0x00C1,
    0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB,
    0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 
    0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF, 
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9,
    0x0054, 0x0000 */

};
static void Test_UChar_WCHART_API(void){
    UErrorCode err = U_ZERO_ERROR;
    const UChar* uSrc = src16j;
    int32_t uSrcLen = sizeof(src16j)/2;
    wchar_t* wDest = NULL;
    int32_t wDestLen = 0;
    int32_t reqLen= 0 ;
    UBool failed = FALSE;
    UChar* uDest = NULL;
    int32_t uDestLen = 0;
    int i =0;
    {
        /* pre-flight*/
        u_strToWCS(wDest,wDestLen,&reqLen,uSrc,uSrcLen-1,&err);

        if(err == U_BUFFER_OVERFLOW_ERROR){
            err=U_ZERO_ERROR;
            wDest =(wchar_t*) uprv_malloc(sizeof(wchar_t) * (reqLen+1));
            wDestLen = reqLen+1;
            u_strToWCS(wDest,wDestLen,&reqLen,uSrc,uSrcLen-1,&err);
        }

        /* pre-flight */
        u_strFromWCS(uDest, uDestLen,&reqLen,wDest,reqLen,&err);

        if(err == U_BUFFER_OVERFLOW_ERROR){
            err =U_ZERO_ERROR;
            uDest = (UChar*) uprv_malloc(sizeof(UChar) * (reqLen+1));
            uDestLen = reqLen + 1;
            u_strFromWCS(uDest, uDestLen,&reqLen,wDest,reqLen,&err);
        }
    

        for(i=0; i< uSrcLen; i++){
            if(uDest[i] != src16j[i]){
                log_verbose("u_str*WCS() failed for unterminated string expected: \\u%04X got: \\u%04X at index: %i \n", src16j[i] ,uDest[i],i);
                failed =TRUE;
            }
        }

        if(U_FAILURE(err)){
            failed = TRUE;
        }
        if(failed){
            log_err("u_strToWCS() failed \n");
        }
        uprv_free(wDest);
        uprv_free(uDest);
        
       
        /* test with embeded nulls */
        uSrc = src16WithNulls;
        uSrcLen = sizeof(src16WithNulls)/2;
        wDestLen =0;
        uDestLen =0;
        wDest = NULL;
        uDest = NULL;
        /* pre-flight*/
        u_strToWCS(wDest,wDestLen,&reqLen,uSrc,uSrcLen-1,&err);

        if(err == U_BUFFER_OVERFLOW_ERROR){
            err=U_ZERO_ERROR;
            wDest =(wchar_t*) uprv_malloc(sizeof(wchar_t) * (reqLen+1));
            wDestLen = reqLen+1;
            u_strToWCS(wDest,wDestLen,&reqLen,uSrc,uSrcLen-1,&err);
        }

        /* pre-flight */
        u_strFromWCS(uDest, uDestLen,&reqLen,wDest,reqLen,&err);

        if(err == U_BUFFER_OVERFLOW_ERROR){
            err =U_ZERO_ERROR;
            uDest = (UChar*) uprv_malloc(sizeof(UChar) * (reqLen+1));
            uDestLen = reqLen + 1;
            u_strFromWCS(uDest, uDestLen,&reqLen,wDest,reqLen,&err);
        }
    

        for(i=0; i< uSrcLen; i++){
            if(uDest[i] != src16WithNulls[i]){
                log_verbose("u_str*WCS() failed for string with nulls expected: \\u%04X got: \\u%04X at index: %i \n", src16WithNulls[i] ,uDest[i],i);
                failed =TRUE;
            }
        }

        if(U_FAILURE(err)){
            failed = TRUE;
        }
        if(failed){
            log_err("u_strToWCS() failed \n");
        }
        uprv_free(wDest);
        uprv_free(uDest);

    }

    {
        
        uSrc = src16j;
        uSrcLen = sizeof(src16j)/2;
        wDestLen =0;
        uDestLen =0;
        wDest = NULL;
        uDest = NULL;
        wDestLen = 0;
        /* pre-flight*/
        u_strToWCS(wDest,wDestLen,&reqLen,uSrc,-1,&err);

        if(err == U_BUFFER_OVERFLOW_ERROR){
            err=U_ZERO_ERROR;
            wDest =(wchar_t*) uprv_malloc(sizeof(wchar_t) * (reqLen+1));
            wDestLen = reqLen+1;
            u_strToWCS(wDest,wDestLen,&reqLen,uSrc,-1,&err);
        }
        uDestLen = 0;
        /* pre-flight */
        u_strFromWCS(uDest, uDestLen,&reqLen,wDest,-1,&err);

        if(err == U_BUFFER_OVERFLOW_ERROR){
            err =U_ZERO_ERROR;
            uDest = (UChar*) uprv_malloc(sizeof(UChar) * (reqLen+1));
            uDestLen = reqLen + 1;
            u_strFromWCS(uDest, uDestLen,&reqLen,wDest,-1,&err);
        }
    

        for(i=0; i< uSrcLen; i++){
            if(uDest[i] != src16j[i]){
                log_verbose("u_str*WCS() failed for null terminated string expected: \\u%04X got: \\u%04X at index: %i \n", src16j[i] ,uDest[i],i);
                failed =TRUE;
            }
        }

        if(U_FAILURE(err)){
            failed = TRUE;
        }
        if(failed){
            log_err("u_strToWCS() failed \n");
        }
        uprv_free(wDest);
        uprv_free(uDest);
    }

} 

  