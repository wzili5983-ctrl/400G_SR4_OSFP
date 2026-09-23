#include <windows.h>
#include <windowsx.h>
#ifdef WIN16   
  #include <ole2.h>
  #include <compobj.h>    
  #include <dispatch.h> 
  #include <variant.h>
  #include <olenls.h> 
#endif
#include <stdarg.h>  


#include <iostream>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* FPGA MDIO burst read can read up to 2048 registers */
#define BURST_READ_MAX_SIZE 2048

#ifdef WIN16
  #define LPCTSTR LPCSTR
  #define LPOLESTR LPSTR
#endif


HRESULT CountArgsInFormat(LPCTSTR pszFmt, UINT  *pn);
LPCTSTR GetNextVarType(LPCTSTR pszFmt, VARTYPE * pvt);
HRESULT CreateObject(LPOLESTR pszProgID, IDispatch ** ppdisp);
HRESULT Invoke(LPDISPATCH pdisp, WORD wFlags, LPVARIANT pvRet, EXCEPINFO FAR* pexcepinfo, UINT FAR* pnArgErr, LPOLESTR pszName, LPCTSTR pszFmt, ...);


#include "MON2_HSIP_ENG.h"



VARIANTARG myBuff[200];
//Monitor_EDC40_ENG::_AppPtr app;
LPDISPATCH pdispMon = NULL;
LPDISPATCH pdispApp = NULL;
VARIANT phy;
VARIANT dev;
VARIANT reg;
VARIANT data;
VARIANT adr;
VARIANT val;
VARIANT ret;
LPSAFEARRAY mPob;
LPSAFEARRAY mPib;

void sal_init(void)
{
    HRESULT hr;
    VARIANT  vRet;
    
    OleInitialize(NULL);

    VariantInit(&phy);
    phy.vt = VT_INT;
    VariantInit(&dev);
    dev.vt = VT_INT;
    VariantInit(&reg);
    reg.vt = VT_INT;
    VariantInit(&data);
    data.vt = VT_INT;
    VariantInit(&adr);
    adr.vt = VT_INT;
    VariantInit(&val);
    val.vt = VT_INT;
    VariantInit(&ret);
    ret.vt = VT_INT;

    hr = CreateObject(OLESTR("Monitor2_HSIP_ENG.Monitor"), &pdispMon); 
    hr = Invoke(pdispMon, DISPATCH_METHOD, &vRet, NULL, NULL,OLESTR("ApplicationAPI"), NULL);
    pdispApp = V_DISPATCH(&vRet);
    //hr = CreateObject(OLESTR("Monitor2_HSIP_ENG.Monitor"), &pdispApp); 

    SAFEARRAYBOUND bound;
    bound.lLbound = 0;
    bound.cElements = BURST_READ_MAX_SIZE*4+1;		// allocate 2048 word buffer for burst read
    mPob = SafeArrayCreate (VT_VARIANT, 1, &bound);
    SAFEARRAYBOUND ibound;
    ibound.lLbound = 0;
    ibound.cElements = BURST_READ_MAX_SIZE*4+1;		// allocate 2048 word buffer for burst read
    mPib = SafeArrayCreate (VT_VARIANT, 1, &ibound);
}

void sal_fini(void)
{
    OleUninitialize();
}

/* 0 - settings panel off
   1 - settings panel on
 */
void sal_settingView(unsigned int view_on)
{
    VARIANT s;
    V_VT(&s) = VT_INT;
    V_INT(&s) = view_on;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("SettingView"), TEXT("v"), s);
}

int sal_rdmdio(int phyad, int devad, int regad)
{
    //phy.intVal = phyad;
    //dev.intVal = devad;
    //reg.intVal = regad;
    //data = app->readMdio(&phy, &dev, &reg);
    //
    //return data.intVal;
    VARIANT p, d, r, val;
    V_VT(&p) = VT_INT;
    V_INT(&p) = phyad;
    V_VT(&d) = VT_INT;
    V_INT(&d) = devad;
    V_VT(&r) = VT_INT;
    V_INT(&r) = regad;
    V_VT(&val) = VT_INT;
    V_INT(&val) = 0;
    Invoke(pdispApp, DISPATCH_METHOD, &val, NULL, NULL, OLESTR("readMdio"), TEXT("vvv"), p, d,r);
    return val.intVal;
}

unsigned int sal_rdEst(int phyad, unsigned int regad)
{
    VARIANT p, r, val;
    V_VT(&p) = VT_INT;
    V_INT(&p) = phyad;
    V_VT(&r) = VT_UINT;
    V_INT(&r) = regad;
    V_VT(&val) = VT_UINT;
    V_INT(&val) = 0;
    Invoke(pdispApp, DISPATCH_METHOD, &val, NULL, NULL, OLESTR("readEst"), TEXT("vv"), p,r);
    return (unsigned int) val.intVal;
}


void sal_writemdio45(int phyad, int devad, int v)
{
    VARIANT p, d, val;
    V_VT(&p) = VT_INT;
    V_INT(&p) = phyad;
    V_VT(&d) = VT_INT;
    V_INT(&d) = devad;
    V_VT(&val) = VT_INT;
    V_INT(&val) = v;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("WriteMdio45"), TEXT("vvv"), p, d,val);
}

void sal_addrmdio45(int phyad, int devad, int regad)
{
    VARIANT p, d, r;
    V_VT(&p) = VT_INT;
    V_INT(&p) = phyad;
    V_VT(&d) = VT_INT;
    V_INT(&d) = devad;
    V_VT(&r) = VT_INT;
    V_INT(&r) = regad;

    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("AddrMdio45"), TEXT("vvv"), p, d,r);   
}

int sal_readmdio45(int phyad, int devad)
{
    VARIANT p, d, val;
    V_VT(&p) = VT_INT;
    V_INT(&p) = phyad;
    V_VT(&d) = VT_INT;
    V_INT(&d) = devad;
    V_VT(&val) = VT_INT;
    V_INT(&val) = 0;
    Invoke(pdispApp, DISPATCH_METHOD, &val, NULL, NULL, OLESTR("ReadMdio45"), TEXT("vv"), p, d);
    return val.intVal;
}

void sal_wrmdio(int phyad, int devad, int regad, int v)
{
    //phy.intVal = phyad;
    //dev.intVal = devad;
    //reg.intVal = regad;
    //data.intVal = v;
    //app->writeMdio(&phy, &dev, &reg, &data);

    VARIANT p, d, r, val;
    V_VT(&p) = VT_INT;
    V_INT(&p) = phyad;
    V_VT(&d) = VT_INT;
    V_INT(&d) = devad;
    V_VT(&r) = VT_INT;
    V_INT(&r) = regad;
    V_VT(&val) = VT_INT;
    V_INT(&val) = v;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("writeMdio"), TEXT("vvvv"), p, d,r,val);
}

/* access mode: 
   0 = MDIO
   1 = I2C
   2 = I2C_BCM81188
   3 = I2C_BCM81181 */
void sal_setAccMdEst(unsigned int access_mode)
{
    VARIANT p;
    V_VT(&p) = VT_INT;
    V_INT(&p) = access_mode;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("SetAccMdEst"), TEXT("v"), p); 
}

int sal_getRcmEst()
{
    VARIANT val;
    V_VT(&val) = VT_INT;
    V_INT(&val) = 0;
    Invoke(pdispApp, DISPATCH_METHOD, &val, NULL, NULL, OLESTR("getRcmEst"), TEXT(""));
    return val.intVal;
}

void sal_setI2CDevEst(unsigned int address)
{
    VARIANT p;
    V_VT(&p) = VT_INT;
    V_INT(&p) = address;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("SetI2cDevEst"), TEXT("v"), p); 
}

int sal_getI2CDevEst()
{
    VARIANT val;
    V_VT(&val) = VT_INT;
    V_INT(&val) = 0;
    Invoke(pdispApp, DISPATCH_METHOD, &val, NULL, NULL, OLESTR("getI2cDevEst"), TEXT(""));
    return val.intVal;
}

void sal_wrEst(int phyad, unsigned int regad, unsigned int v)
{
    VARIANT p, r, val;
    V_VT(&p) = VT_INT;
    V_INT(&p) = phyad;
    V_VT(&r) = VT_UINT;
    V_INT(&r) = regad;
    V_VT(&val) = VT_UINT;
    V_INT(&val) = (unsigned int) v;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("writeEst"), TEXT("vvv"), p,r,val);   
}

int sal_rdFPGA(int regad)
{
    VARIANT r, val;
    V_VT(&r) = VT_INT;
    V_INT(&r) = regad;
    V_VT(&val) = VT_INT;
    V_INT(&val) = 0;
    Invoke(pdispApp, DISPATCH_METHOD, &val, NULL, NULL, OLESTR("readFPGA"), TEXT("v"), r);
    return val.intVal;
}
void sal_wrFPGA(int regad, int v)
{
    VARIANT r, val;
    V_VT(&r) = VT_INT;
    V_INT(&r) = regad;
    V_VT(&val) = VT_INT;
    V_INT(&val) = v;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("writeFPGA"), TEXT("vv"), r,val);    
}

void write_pcb(int addr, int wr_data)
{
    VARIANT r, val;
    V_VT(&r) = VT_INT;
    V_INT(&r) = addr;
    V_VT(&val) = VT_INT;
    V_INT(&val) = wr_data;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("writePcb"), TEXT("vv"), r,val);    
}
int read_pcb(int addr)
{
    VARIANT r, val;
    V_VT(&r) = VT_INT;
    V_INT(&r) = addr;
    V_VT(&val) = VT_INT;
    V_INT(&val) = 0;
    Invoke(pdispApp, DISPATCH_METHOD, &val, NULL, NULL, OLESTR("readPcb "), TEXT("v"), r);
    return val.intVal;    
}
void set_com(int port)
{
    VARIANT p;
    V_VT(&p) = VT_INT;
    V_INT(&p) = port;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("SetCom"), TEXT("v"), p);        
}

unsigned char sal_readAhbI2cByte(int dev, int addr)
{
    VARIANT d, a, v;
    V_VT(&d) = VT_INT;
    V_INT(&d) = dev;
    V_VT(&a) = VT_INT;
    V_INT(&a) = addr;
    V_VT(&v) = VT_INT;
    V_INT(&v) = 0;
    Invoke(pdispApp, DISPATCH_METHOD, &v, NULL, NULL, OLESTR("readAhbI2cByte"), TEXT("vv"), d, a);
    return (unsigned char)v.intVal;
}

void sal_writeAhbI2cByte(int dev, int addr, unsigned char val)
{
    VARIANT d, a, v;
    V_VT(&d) = VT_INT;
    V_INT(&d) = dev;
    V_VT(&a) = VT_INT;
    V_INT(&a) = addr;
    V_VT(&v) = VT_INT;
    V_INT(&v) = (int)val;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("writeAhbI2cByte"), TEXT("vvv"), d, a, v);
}

unsigned char sal_readI2c(int dev, int addr)
{
    VARIANT d, a, v;
    V_VT(&d) = VT_INT;
    V_INT(&d) = dev;
    V_VT(&a) = VT_INT;
    V_INT(&a) = addr;
    V_VT(&v) = VT_INT;
    V_INT(&v) = 0;
    Invoke(pdispApp, DISPATCH_METHOD, &v, NULL, NULL, OLESTR("rdI2c"), TEXT("vv"), d, a);
    return (unsigned char)v.intVal;
}

void sal_writeI2c(int dev, int addr, unsigned char val)
{
    VARIANT d, a, v;
    V_VT(&d) = VT_INT;
    V_INT(&d) = dev;
    V_VT(&a) = VT_INT;
    V_INT(&a) = addr;
    V_VT(&v) = VT_INT;
    V_INT(&v) = (int)val;
    Invoke(pdispApp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("wrI2c"), TEXT("vvv"), d, a, v);
}

/*************************************************************************
**
**  This is a part of the Microsoft Source Code Samples.
**
**  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
**
**  This source code is only intended as a supplement to Microsoft Development
**  Tools and/or WinHelp documentation.  See these sources for detailed
**  information regarding the Microsoft samples programs.
**
**  Automation Controller helper functions
**
**  invhelp.cpp
**
**  Written by Microsoft Product Support Services, Windows Developer Support
**
*************************************************************************/


/*
 * CreateObject
 *
 * Purpose:
 *  Creates an instance of the Automation object and obtains it's IDispatch interface.
 *  Uses Unicode with OLE.
 *
 * Parameters:
 *  pszProgID         ProgID of Automation object
 *  ppdisp             Returns IDispatch of Automation object
 *
 * Return Value:
 *  HRESULT indicating success or failure 
 */
HRESULT CreateObject(LPOLESTR pszProgID, IDispatch ** ppdisp)
{
    CLSID clsid;                   // CLSID of automation object
    HRESULT hr;
    LPUNKNOWN punk = NULL;         // IUnknown of automation object
    LPDISPATCH pdisp = NULL;       // IDispatch of automation object
    
    *ppdisp = NULL;
    
    // Retrieve CLSID from the progID that the user specified
    hr = CLSIDFromProgID(pszProgID, &clsid);
    if (FAILED(hr))
        goto error;
    
    // Create an instance of the automation object and ask for the IDispatch interface
    hr = CoCreateInstance(clsid, NULL, CLSCTX_SERVER, 
                          IID_IUnknown, (void FAR* FAR*)&punk);
    if (FAILED(hr))
        goto error;
                   
    hr = punk->QueryInterface(IID_IDispatch, (void FAR* FAR*)&pdisp);
    if (FAILED(hr))
        goto error;

    *ppdisp = pdisp;
    punk->Release();
    return NOERROR;
     
error:
    if (punk) punk->Release();
    if (pdisp) pdisp->Release();
    return hr;
}  

/*
 * Invoke
 *
 * Purpose:
 *  Invokes a property accessor function or method of an automation object. Uses Unicode with OLE.
 *
 * Parameters:
 *  pdisp         IDispatch* of automation object.
 *  wFlags        Specfies if property is to be accessed or method to be invoked.
 *                Can hold DISPATCH_PROPERTYGET, DISPATCH_PROPERTYPUT, DISPATCH_METHOD,
 *                DISPATCH_PROPERTYPUTREF or DISPATCH_PROPERTYGET|DISPATCH_METHOD.   
 *  pvRet         NULL if caller excepts no result. Otherwise returns result.
 *  pexcepinfo    Returns exception info if DISP_E_EXCEPTION is returned. Can be NULL if
 *                caller is not interested in exception information. 
 *  pnArgErr      If return is DISP_E_TYPEMISMATCH, this returns the index (in reverse
 *                order) of argument with incorrect type. Can be NULL if caller is not interested
 *                in this information. 
 *  pszName       Name of property or method.
 *  pszFmt        Format string that describes the variable list of parameters that 
 *                follows. The format string can contain the follwoing characters.
 *                & = mark the following format character as VT_BYREF 
 *                b = VT_BOOL
 *                i = VT_I2
 *                I = VT_I4
 *                r = VT_R2
 *                R = VT_R4
 *                c = VT_CY 
 *                s = VT_BSTR (far string pointer can be passed, BSTR will be allocated by this function).
 *                e = VT_ERROR
 *                d = VT_DATE
 *                v = VT_VARIANT. Use this to pass data types that are not described in 
 *                                the format string. (For example SafeArrays).
 *                D = VT_DISPATCH
 *                U = VT_UNKNOWN
 *    
 *  ...           Arguments of the property or method. Arguments are described by pszFmt.  
 *                ****FAR POINTERS MUST BE PASSED FOR POINTER ARGUMENTS in Win16.****
 * Return Value:
 *  HRESULT indicating success or failure        
 *
 * Usage examples:
 *
 *  HRESULT hr;  
 *  LPDISPATCH pdisp;   
 *  BSTR bstr;
 *  short i;
 *  BOOL b;   
 *  VARIANT v, v2;
 *
 *1. bstr = SysAllocString(OLESTR(""));
 *   hr = Invoke(pdisp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("method1"), 
 *        TEXT("bis&b&i&s"), TRUE, 2, (LPOLESTR)OLESTR("param"), (BOOL FAR*)&b, (short FAR*)&i, (BSTR FAR*)&bstr);   
 *
 *2. VariantInit(&v);
 *   V_VT(&v) = VT_R8;
 *   V_R8(&v) = 12345.6789; 
 *   VariantInit(&v2);
 *   hr = Invoke(pdisp, DISPATCH_METHOD, NULL, NULL, NULL, OLESTR("method2"), 
 *         TEXT("v&v"), v, (VARIANT FAR*)&v2);
 */
HRESULT 
Invoke(LPDISPATCH pdisp, 
    WORD wFlags,
    LPVARIANT pvRet,
    EXCEPINFO FAR* pexcepinfo,
    UINT FAR* pnArgErr, 
    LPOLESTR pszName,
    LPCTSTR pszFmt, 
    ...)
{
    va_list argList;
    va_start(argList, pszFmt);  
    DISPID dispid;
    HRESULT hr;
    VARIANTARG* pvarg = NULL;
  
    if (pdisp == NULL)
        return ResultFromScode(E_INVALIDARG);
    
    // Get DISPID of property/method
    hr = pdisp->GetIDsOfNames(IID_NULL, &pszName, 1, LOCALE_USER_DEFAULT, &dispid);
    if(FAILED(hr))
        return hr;
               
    DISPPARAMS dispparams;
    _fmemset(&dispparams, 0, sizeof dispparams);

    // determine number of arguments
    if (pszFmt != NULL)
        CountArgsInFormat(pszFmt, &dispparams.cArgs);
    
    // Property puts have a named argument that represents the value that the property is
    // being assigned.
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    if (wFlags & DISPATCH_PROPERTYPUT)
    {
        if (dispparams.cArgs == 0)
            return ResultFromScode(E_INVALIDARG);
        dispparams.cNamedArgs = 1;
        dispparams.rgdispidNamedArgs = &dispidNamed;
    }

    if (dispparams.cArgs != 0)
    {
        // allocate memory for all VARIANTARG parameters
        //pvarg = new VARIANTARG[dispparams.cArgs];
        pvarg = myBuff;
        if(pvarg == NULL)
            return ResultFromScode(E_OUTOFMEMORY);   
        dispparams.rgvarg = pvarg;
        _fmemset(pvarg, 0, sizeof(VARIANTARG) * dispparams.cArgs);

        // get ready to walk vararg list
        LPCTSTR psz = pszFmt;
        pvarg += dispparams.cArgs - 1;   // params go in opposite order
        
        while (psz = GetNextVarType(psz, &pvarg->vt))
        {
            if (pvarg < dispparams.rgvarg)
            {
                hr = ResultFromScode(E_INVALIDARG);
                goto cleanup;  
            }
            switch (pvarg->vt)
            {
            case VT_I2:
                V_I2(pvarg) = va_arg(argList, short);
                break;
            case VT_I4:
                V_I4(pvarg) = va_arg(argList, long);
                break;
            case VT_R4:
                V_R4(pvarg) = va_arg(argList, float);
                break; 
            case VT_DATE:
            case VT_R8:
                V_R8(pvarg) = va_arg(argList, double);
                break;
            case VT_CY:
                V_CY(pvarg) = va_arg(argList, CY);
                break;
            case VT_BSTR:
                V_BSTR(pvarg) = SysAllocString(va_arg(argList, OLECHAR FAR*));
                if (pvarg->bstrVal == NULL) 
                {
                    hr = ResultFromScode(E_OUTOFMEMORY);  
                    pvarg->vt = VT_EMPTY;
                    goto cleanup;  
                }
                break;
            case VT_DISPATCH:
                V_DISPATCH(pvarg) = va_arg(argList, LPDISPATCH);
                break;
            case VT_ERROR:
                V_ERROR(pvarg) = va_arg(argList, SCODE);
                break;
            case VT_BOOL:
                V_BOOL(pvarg) = va_arg(argList, BOOL) ? -1 : 0;
                break;
            case VT_VARIANT:
                *pvarg = va_arg(argList, VARIANTARG); 
                break;
            case VT_UNKNOWN:
                V_UNKNOWN(pvarg) = va_arg(argList, LPUNKNOWN);
                break;

            case VT_I2|VT_BYREF:
                V_I2REF(pvarg) = va_arg(argList, short FAR*);
                break;
            case VT_I4|VT_BYREF:
                V_I4REF(pvarg) = va_arg(argList, long FAR*);
                break;
            case VT_R4|VT_BYREF:
                V_R4REF(pvarg) = va_arg(argList, float FAR*);
                break;
            case VT_R8|VT_BYREF:
                V_R8REF(pvarg) = va_arg(argList, double FAR*);
                break;
            case VT_DATE|VT_BYREF:
                V_DATEREF(pvarg) = va_arg(argList, DATE FAR*);
                break;
            case VT_CY|VT_BYREF:
                V_CYREF(pvarg) = va_arg(argList, CY FAR*);
                break;
            case VT_BSTR|VT_BYREF:
                V_BSTRREF(pvarg) = va_arg(argList, BSTR FAR*);
                break;
            case VT_DISPATCH|VT_BYREF:
                V_DISPATCHREF(pvarg) = va_arg(argList, LPDISPATCH FAR*);
                break;
            case VT_ERROR|VT_BYREF:
                V_ERRORREF(pvarg) = va_arg(argList, SCODE FAR*);
                break;
            case VT_BOOL|VT_BYREF: 
                {
                    BOOL FAR* pbool = va_arg(argList, BOOL FAR*);
                    *pbool = 0;
                    V_BOOLREF(pvarg) = (VARIANT_BOOL FAR*)pbool;
                } 
                break;              
            case VT_VARIANT|VT_BYREF: 
                V_VARIANTREF(pvarg) = va_arg(argList, VARIANTARG FAR*);
                break;
            case VT_UNKNOWN|VT_BYREF:
                V_UNKNOWNREF(pvarg) = va_arg(argList, LPUNKNOWN FAR*);
                break;

            default:
                {
                    hr = ResultFromScode(E_INVALIDARG);
                    goto cleanup;  
                }
                break;
            }

            --pvarg; // get ready to fill next argument
        } //while

    } //if
   
    // Initialize return variant, in case caller forgot. Caller can pass NULL if return
    // value is not expected.
    if (pvRet)
        VariantInit(pvRet); 
    // make the call 
    hr = pdisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, wFlags,
        &dispparams, pvRet, pexcepinfo, pnArgErr);

cleanup:
    // cleanup any arguments that need cleanup
    if (dispparams.cArgs != 0)
    {
        VARIANTARG FAR* pvarg = dispparams.rgvarg;
        UINT cArgs = dispparams.cArgs;   
        
        while (cArgs--)
        {
            switch (pvarg->vt)
            {
            case VT_BSTR:
                VariantClear(pvarg);
                break;
            }
            ++pvarg;
        }
    }
    //delete dispparams.rgvarg;
    va_end(argList);
    return hr;   
}   

HRESULT CountArgsInFormat(LPCTSTR pszFmt, UINT *pn)
{
    *pn = 0;

    if(pszFmt == NULL)
      return NOERROR;
    
    while (*pszFmt)  
    {
       if (*pszFmt == '&')
           pszFmt++;

       switch(*pszFmt)
       {
           case 'b':
           case 'i': 
           case 'I':
           case 'r': 
           case 'R':
           case 'c':
           case 's':
           case 'e':
           case 'd':
           case 'v':
           case 'D':
           case 'U':
               ++*pn; 
               pszFmt++;
               break;
           case '\0':  
           default:
               return ResultFromScode(E_INVALIDARG);   
        }
    }
    return NOERROR;
}


LPCTSTR GetNextVarType(LPCTSTR pszFmt, VARTYPE * pvt)
{   
    *pvt = 0;
    if (*pszFmt == '&') 
    {
         *pvt = VT_BYREF; 
         pszFmt++;    
         if (!*pszFmt)
             return NULL;    
    } 
    switch(*pszFmt)
    {
        case 'b':
            *pvt |= VT_BOOL;
            break;
        case 'i': 
            *pvt |= VT_I2;
            break;
        case 'I': 
            *pvt |= VT_I4;
            break;
        case 'r': 
            *pvt |= VT_R4;
            break;
        case 'R': 
            *pvt |= VT_R8;
            break;
        case 'c':
            *pvt |= VT_CY;
            break;
        case 's': 
            *pvt |= VT_BSTR;
            break;
        case 'e': 
            *pvt |= VT_ERROR;
            break;
        case 'd': 
            *pvt |= VT_DATE; 
            break;
        case 'v': 
            *pvt |= VT_VARIANT;
            break;
        case 'U': 
            *pvt |= VT_UNKNOWN; 
            break;
        case 'D': 
            *pvt |= VT_DISPATCH;
            break;  
        case '\0':
             return NULL;     // End of Format string
        default:
            return NULL;
    } 
    return ++pszFmt;  
}



void sal_send2usb(int ocnt, unsigned char *obuff, int icnt, unsigned char *ibuff)
{
       
 
       LONG i;
      
       HRESULT hr;
       DISPID dispid;
       LPOLESTR pszName = OLESTR("send2usb");
 
       // Get DISPID of property/method
    hr = pdispApp->GetIDsOfNames(IID_NULL,&pszName, 1, LOCALE_USER_DEFAULT, &dispid);
    if(FAILED(hr))
        return;
 
       VARIANTARG v[4];    //oc, ob, ic, ib
       //oc
       
       v[3].vt = VT_INT;   
       v[3].intVal = ocnt;
       //ob
       //VariantInit(&v[2]);
       v[2].vt = VT_ARRAY | VT_VARIANT; 
       //SAFEARRAY* pob;
       LPSAFEARRAY pob;
    SAFEARRAYBOUND bound;
       bound.lLbound = 0;
       bound.cElements = ocnt;
       pob = SafeArrayCreate (VT_VARIANT, 1, &bound);
 
       VARIANT HUGEP * pData = NULL;
       hr = SafeArrayAccessData(pob, (void HUGEP * FAR *)&pData);
       for (i= 0;i<ocnt; i++, pData++)
       {
        VariantInit(pData);
        pData->vt = VT_UI1;
        pData->bVal = *(obuff+i);
       }
       //SafeArrayUnaccessData(pob);
       /*for (i=0; i<ocnt; i++)
        SafeArrayPutElement(pob, &i, (obuff+i));*/
       v[2].parray = pob;
       //ic
       //VariantInit(&v[1]);
       v[1].vt = VT_INT;   
       v[1].intVal = icnt;
       //ib
       //VariantInit(&v[0]);
       v[0].vt = VT_ARRAY | VT_VARIANT|VT_BYREF;
       SAFEARRAY* pib;
    SAFEARRAYBOUND ibound;
       ibound.lLbound = 0;
       ibound.cElements = icnt;
       pib = SafeArrayCreate (VT_VARIANT, 1, &ibound);
 
       VARIANT HUGEP * pData2 = NULL;
       hr = SafeArrayAccessData(pib, (void HUGEP * FAR *)&pData2);
       for (i= 0;i<icnt; i++, pData2++)
       {
        VariantInit(pData2);
        pData2->vt = VT_UI1;
        pData2->bVal = (BYTE)i;
       }
       //SafeArrayUnaccessData(pib);
       /*for (i=0; i<icnt; i++)
        SafeArrayPutElement(pib, &i, &i);*/
       v[0].pparray = &pib;
 
 
    // Contains the arguments passed to a method or property.
    DISPPARAMS params = {v, NULL, 4, 0};
 
       VariantInit(&ret); 
       V_VT(&ret) = VT_INT;
       V_INT(&ret) = 0;
 
       // To catch errors
    EXCEPINFO pExcepInfo;
       UINT nArgErr = 10;
       // make the call 
    hr = pdispApp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD,  &params, &ret, &pExcepInfo, &nArgErr);
 
       if(FAILED(hr)) {
        if(hr == DISP_E_EXCEPTION) {
            printf("%S\n", pExcepInfo.bstrSource);
            printf("%S\n", pExcepInfo.bstrDescription);
        }
 
    }
 
       /*for (i=0; i<icnt; i++)
        hr = SafeArrayGetElement(pib, &i, &ibuff[i]);*/
       hr = SafeArrayAccessData(pib, (void HUGEP * FAR *)&pData2);
       for (i= 0;i<icnt; i++, pData2++)
       {
        ibuff[i] = pData2->bVal;
       }

       //VariantClear(pData);
       //VariantClear(pData2);

	   SafeArrayUnaccessData(pob);
	   SafeArrayUnaccessData(pib);

       SafeArrayDestroy(pob);
       SafeArrayDestroy(pib);
       
    // Clears the variant.
    hr = VariantClear(v);
}


int sal_getAccMdEst()
{

       VARIANT val;
       V_VT(&val) = VT_INT;
       V_INT(&val) = 0;
       Invoke(pdispApp, DISPATCH_METHOD, &val, NULL, NULL, OLESTR("getAccMdEst"), TEXT(""));

       return val.intVal;
}

//Function App.sal_readEstInc ( ByVal prtad, ByVal addr, ByVal cnt, ByRef data  )
//<summary>: Read data from Estoque with Inc method, MDIO only . return -1, if error
//<input>: prtad
//<input>: addr
//<input>: cnt:number of 32bit word to read
//<input>: data: memory point to data buffer
//<return>: Error Code (0 means no error)
int sal_readEstInc(int prtad, unsigned int addr, int cnt, unsigned int *data)
{
    LONG i;
    HRESULT hr;
    DISPID dispid;
    LPOLESTR pszName = OLESTR("readEstInc");
 
    if (cnt > BURST_READ_MAX_SIZE)
        return -1;

    // Get DISPID of property/method
    hr = pdispApp->GetIDsOfNames(IID_NULL,&pszName, 1, LOCALE_USER_DEFAULT, &dispid);
    if(FAILED(hr))
        return -2;
 
    VARIANTARG v[4];    //prtad, addr,cnt, data
                        //3    , 2   ,  1,    0

    v[3].vt = VT_INT;   
    v[3].intVal = prtad;
    v[2].vt = VT_UINT;   
    v[2].intVal = addr;
    v[1].vt = VT_INT;   
    v[1].intVal = cnt;
    v[0].vt = VT_ARRAY | VT_VARIANT|VT_BYREF; 
    VARIANT HUGEP * pData2 = NULL;
    hr = SafeArrayAccessData(mPib, (void HUGEP * FAR *)&pData2);
    if(FAILED(hr)) {
        printf("SafeArrayAccessData return error %d \n", hr);
        return -4; 
    }
	
    for (i= 0;i<cnt; i++, pData2++)
    {
        VariantInit(pData2);
        pData2->vt = VT_UINT;
        pData2->intVal = i;
    }
    v[0].pparray = &mPib;      
       
 
 
    // Contains the arguments passed to a method or property.
    DISPPARAMS params = {v, NULL, 4, 0};
 
    VariantInit(&ret); 
    V_VT(&ret) = VT_INT;
    V_INT(&ret) = 0;
 
    // To catch errors
    EXCEPINFO pExcepInfo;
    UINT nArgErr = 10;

    // make the call 
    hr = pdispApp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD,  &params, &ret, &pExcepInfo, &nArgErr);

    SafeArrayUnaccessData(mPib);
 
    if(FAILED(hr)) {
        printf("readEstInc return error %d \n", hr);
        if(hr == DISP_E_EXCEPTION) {
            //printf("%S\n", pExcepInfo.bstrSource);
            //printf("%S\n", pExcepInfo.bstrDescription);
        }
        return -3; 
    }

    hr = SafeArrayAccessData(mPib, (void HUGEP * FAR *)&pData2);
    if(FAILED(hr)) {
        printf("SafeArrayAccessData return error %d \n", hr);
        return -5; 
    }
	
    for (i= 0;i<cnt; i++, pData2++)
    {
        data[i] = (unsigned int)pData2->intVal;
    }

    SafeArrayUnaccessData(mPib);
	
    // Clears the variant.
    hr = VariantClear(v);
    return 0;
}

//Function App.sal_writeEstInc ( ByVal prtad, ByVal addr, ByVal wcnt, ByVal data  )
//<summary>: Write data from Estoque with Inc method, MDIO only . return -1, if error
//<input>: prtad
//<input>: addr
//<input>: cnt:number of 32bit word to read
//<input>: data: memory point to data buffer
//<return>: Error Code (0 means no error)
int sal_writeEstInc(int prtad, unsigned int addr, int cnt, unsigned int *data)
{ 
    LONG i;
    HRESULT hr;
    DISPID dispid;
    LPOLESTR pszName = OLESTR("writeEstInc");

    if (cnt > BURST_READ_MAX_SIZE)
        return -1;

    // Get DISPID of property/method
    hr = pdispApp->GetIDsOfNames(IID_NULL,&pszName, 1, LOCALE_USER_DEFAULT, &dispid);
    if(FAILED(hr))
        return -2;
 
  
    VARIANTARG v[4];    //prtad, addr,cnt, data
                        //3    , 2   ,  1,    0

    v[3].vt = VT_INT;   
    v[3].intVal = prtad;
    v[2].vt = VT_UINT;   
    v[2].intVal = addr;
    v[1].vt = VT_INT;   
    v[1].intVal = cnt;
    v[0].vt = VT_ARRAY | VT_VARIANT|VT_BYREF; 
    VARIANT HUGEP * pData2 = NULL;
    hr = SafeArrayAccessData(mPib, (void HUGEP * FAR *)&pData2);
    if(FAILED(hr)) {
        printf("SafeArrayAccessData return error %d \n", hr);
        return -4; 
    }
	
    for (i= 0;i<cnt; i++, pData2++)
    {
        VariantInit(pData2);
        pData2->vt = VT_UINT;
        pData2->intVal = data[i];
    }
    v[0].pparray = &mPib;      
       
 
 
    // Contains the arguments passed to a method or property.
    DISPPARAMS params = {v, NULL, 4, 0};
 
    VariantInit(&ret); 
    V_VT(&ret) = VT_INT;
    V_INT(&ret) = 0;
 
    // To catch errors
    EXCEPINFO pExcepInfo;
    UINT nArgErr = 10;
    // make the call 
    hr = pdispApp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD,  &params, &ret, &pExcepInfo, &nArgErr);

    SafeArrayUnaccessData(mPib);
 
    if(FAILED(hr)) {
        printf("writeEstInc return error %d \n", hr);
        if(hr == DISP_E_EXCEPTION) {
            //printf("%S\n", pExcepInfo.bstrSource);
            //printf("%S\n", pExcepInfo.bstrDescription);
        }
        return -3;
    }
       
    // Clears the variant.
    hr = VariantClear(v);
    return 0;
}
