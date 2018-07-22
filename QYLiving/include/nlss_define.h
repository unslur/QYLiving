#ifndef _NLSS_DEFINE_H_
#define _NLSS_DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__cdecl)
#define EXTAPI __cdecl
#else
#define EXTAPI
#endif

#define DLLDEMO1_EXPORTS
#ifdef DLLDEMO1_EXPORTS
#define EXPORTS_API _declspec( dllexport )
#else
#define EXPORTS_API _declspec(dllimport)
#endif

/*! \file */

#define  NLSS_MASK_HANDLE_TYPE(_name)  struct stru_##_name##__ {int iUnused; } * 
#define  NLSS_MASK_TYPE(_name, _type)  struct stru_##_name##_type {int iUnused; } *

/***NLSS ��������ֵ***/
typedef  NLSS_MASK_TYPE(_RET, int)      NLSS_RET;
#define  NLSS_OK                       (NLSS_RET)0
#define  NLSS_ERR                      (NLSS_RET)1
/***NLSS ���α�ʶ***/
#define  NLSS_OUT

#ifdef __cplusplus
}
#endif

#endif//_LS_MEDIALIVESTREAMING_H_
