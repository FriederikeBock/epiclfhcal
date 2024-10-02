// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Cint
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "Setup.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Setup*)
   {
      ::Setup *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Setup >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Setup", ::Setup::Class_Version(), "Setup.h", 7,
                  typeid(::Setup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Setup::Dictionary, isa_proxy, 4,
                  sizeof(::Setup) );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Setup*)
   {
      return GenerateInitInstanceLocal(static_cast<::Setup*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Setup*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Setup::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Setup::Class_Name()
{
   return "Setup";
}

//______________________________________________________________________________
const char *Setup::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Setup*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Setup::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Setup*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Setup::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Setup*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Setup::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Setup*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Setup::Streamer(TBuffer &R__b)
{
   // Stream an object of class Setup.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Setup::Class(),this);
   } else {
      R__b.WriteClassBuffer(Setup::Class(),this);
   }
}

namespace ROOT {
} // end of namespace ROOT for class ::Setup

namespace {
  void TriggerDictionaryInitialization_Cint_Impl() {
    static const char* headers[] = {
"Setup.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/vandrieu/Software/Root/Myinstall/include/",
"/home/vandrieu/Software/epiclfhcal/NewStructure/Tests/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Cint dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$Setup.h")))  Setup;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Cint dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Setup.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Setup", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Cint",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Cint_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Cint_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Cint() {
  TriggerDictionaryInitialization_Cint_Impl();
}
