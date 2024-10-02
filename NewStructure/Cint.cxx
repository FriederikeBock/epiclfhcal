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
#include "RootSetupWrapper.h"
#include "Calib.h"
#include "Setup.h"
#include "Event.h"
#include "Tile.h"
#include "HGCROC.h"
#include "Caen.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_RootSetupWrapper(void *p = nullptr);
   static void *newArray_RootSetupWrapper(Long_t size, void *p);
   static void delete_RootSetupWrapper(void *p);
   static void deleteArray_RootSetupWrapper(void *p);
   static void destruct_RootSetupWrapper(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RootSetupWrapper*)
   {
      ::RootSetupWrapper *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RootSetupWrapper >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("RootSetupWrapper", ::RootSetupWrapper::Class_Version(), "RootSetupWrapper.h", 10,
                  typeid(::RootSetupWrapper), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RootSetupWrapper::Dictionary, isa_proxy, 4,
                  sizeof(::RootSetupWrapper) );
      instance.SetNew(&new_RootSetupWrapper);
      instance.SetNewArray(&newArray_RootSetupWrapper);
      instance.SetDelete(&delete_RootSetupWrapper);
      instance.SetDeleteArray(&deleteArray_RootSetupWrapper);
      instance.SetDestructor(&destruct_RootSetupWrapper);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RootSetupWrapper*)
   {
      return GenerateInitInstanceLocal(static_cast<::RootSetupWrapper*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::RootSetupWrapper*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Setup*)
   {
      ::Setup *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Setup >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Setup", ::Setup::Class_Version(), "Setup.h", 10,
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

namespace ROOT {
   static void *new_Calib(void *p = nullptr);
   static void *newArray_Calib(Long_t size, void *p);
   static void delete_Calib(void *p);
   static void deleteArray_Calib(void *p);
   static void destruct_Calib(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Calib*)
   {
      ::Calib *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Calib >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Calib", ::Calib::Class_Version(), "Calib.h", 10,
                  typeid(::Calib), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Calib::Dictionary, isa_proxy, 4,
                  sizeof(::Calib) );
      instance.SetNew(&new_Calib);
      instance.SetNewArray(&newArray_Calib);
      instance.SetDelete(&delete_Calib);
      instance.SetDeleteArray(&deleteArray_Calib);
      instance.SetDestructor(&destruct_Calib);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Calib*)
   {
      return GenerateInitInstanceLocal(static_cast<::Calib*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Calib*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Tile(void *p = nullptr);
   static void *newArray_Tile(Long_t size, void *p);
   static void delete_Tile(void *p);
   static void deleteArray_Tile(void *p);
   static void destruct_Tile(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Tile*)
   {
      ::Tile *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Tile >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Tile", ::Tile::Class_Version(), "Tile.h", 11,
                  typeid(::Tile), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Tile::Dictionary, isa_proxy, 4,
                  sizeof(::Tile) );
      instance.SetNew(&new_Tile);
      instance.SetNewArray(&newArray_Tile);
      instance.SetDelete(&delete_Tile);
      instance.SetDeleteArray(&deleteArray_Tile);
      instance.SetDestructor(&destruct_Tile);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Tile*)
   {
      return GenerateInitInstanceLocal(static_cast<::Tile*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Tile*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Caen(void *p = nullptr);
   static void *newArray_Caen(Long_t size, void *p);
   static void delete_Caen(void *p);
   static void deleteArray_Caen(void *p);
   static void destruct_Caen(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Caen*)
   {
      ::Caen *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Caen >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Caen", ::Caen::Class_Version(), "Caen.h", 6,
                  typeid(::Caen), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Caen::Dictionary, isa_proxy, 4,
                  sizeof(::Caen) );
      instance.SetNew(&new_Caen);
      instance.SetNewArray(&newArray_Caen);
      instance.SetDelete(&delete_Caen);
      instance.SetDeleteArray(&deleteArray_Caen);
      instance.SetDestructor(&destruct_Caen);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Caen*)
   {
      return GenerateInitInstanceLocal(static_cast<::Caen*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Caen*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Hgcroc(void *p = nullptr);
   static void *newArray_Hgcroc(Long_t size, void *p);
   static void delete_Hgcroc(void *p);
   static void deleteArray_Hgcroc(void *p);
   static void destruct_Hgcroc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Hgcroc*)
   {
      ::Hgcroc *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Hgcroc >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("Hgcroc", ::Hgcroc::Class_Version(), "HGCROC.h", 7,
                  typeid(::Hgcroc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Hgcroc::Dictionary, isa_proxy, 4,
                  sizeof(::Hgcroc) );
      instance.SetNew(&new_Hgcroc);
      instance.SetNewArray(&newArray_Hgcroc);
      instance.SetDelete(&delete_Hgcroc);
      instance.SetDeleteArray(&deleteArray_Hgcroc);
      instance.SetDestructor(&destruct_Hgcroc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Hgcroc*)
   {
      return GenerateInitInstanceLocal(static_cast<::Hgcroc*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Hgcroc*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *Event_Dictionary();
   static void Event_TClassManip(TClass*);
   static void *new_Event(void *p = nullptr);
   static void *newArray_Event(Long_t size, void *p);
   static void delete_Event(void *p);
   static void deleteArray_Event(void *p);
   static void destruct_Event(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Event*)
   {
      ::Event *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Event));
      static ::ROOT::TGenericClassInfo 
         instance("Event", "Event.h", 10,
                  typeid(::Event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Event_Dictionary, isa_proxy, 4,
                  sizeof(::Event) );
      instance.SetNew(&new_Event);
      instance.SetNewArray(&newArray_Event);
      instance.SetDelete(&delete_Event);
      instance.SetDeleteArray(&deleteArray_Event);
      instance.SetDestructor(&destruct_Event);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Event*)
   {
      return GenerateInitInstanceLocal(static_cast<::Event*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::Event*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Event_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::Event*>(nullptr))->GetClass();
      Event_TClassManip(theClass);
   return theClass;
   }

   static void Event_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr RootSetupWrapper::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *RootSetupWrapper::Class_Name()
{
   return "RootSetupWrapper";
}

//______________________________________________________________________________
const char *RootSetupWrapper::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RootSetupWrapper*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int RootSetupWrapper::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RootSetupWrapper*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RootSetupWrapper::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RootSetupWrapper*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RootSetupWrapper::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RootSetupWrapper*)nullptr)->GetClass(); }
   return fgIsA;
}

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
atomic_TClass_ptr Calib::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Calib::Class_Name()
{
   return "Calib";
}

//______________________________________________________________________________
const char *Calib::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Calib*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Calib::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Calib*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Calib::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Calib*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Calib::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Calib*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Tile::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Tile::Class_Name()
{
   return "Tile";
}

//______________________________________________________________________________
const char *Tile::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tile*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Tile::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Tile*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Tile::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tile*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Tile::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Tile*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Caen::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Caen::Class_Name()
{
   return "Caen";
}

//______________________________________________________________________________
const char *Caen::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Caen*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Caen::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Caen*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Caen::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Caen*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Caen::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Caen*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Hgcroc::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Hgcroc::Class_Name()
{
   return "Hgcroc";
}

//______________________________________________________________________________
const char *Hgcroc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Hgcroc*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Hgcroc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Hgcroc*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Hgcroc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Hgcroc*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Hgcroc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Hgcroc*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void RootSetupWrapper::Streamer(TBuffer &R__b)
{
   // Stream an object of class RootSetupWrapper.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RootSetupWrapper::Class(),this);
   } else {
      R__b.WriteClassBuffer(RootSetupWrapper::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RootSetupWrapper(void *p) {
      return  p ? new(p) ::RootSetupWrapper : new ::RootSetupWrapper;
   }
   static void *newArray_RootSetupWrapper(Long_t nElements, void *p) {
      return p ? new(p) ::RootSetupWrapper[nElements] : new ::RootSetupWrapper[nElements];
   }
   // Wrapper around operator delete
   static void delete_RootSetupWrapper(void *p) {
      delete (static_cast<::RootSetupWrapper*>(p));
   }
   static void deleteArray_RootSetupWrapper(void *p) {
      delete [] (static_cast<::RootSetupWrapper*>(p));
   }
   static void destruct_RootSetupWrapper(void *p) {
      typedef ::RootSetupWrapper current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::RootSetupWrapper

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

//______________________________________________________________________________
void Calib::Streamer(TBuffer &R__b)
{
   // Stream an object of class Calib.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Calib::Class(),this);
   } else {
      R__b.WriteClassBuffer(Calib::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Calib(void *p) {
      return  p ? new(p) ::Calib : new ::Calib;
   }
   static void *newArray_Calib(Long_t nElements, void *p) {
      return p ? new(p) ::Calib[nElements] : new ::Calib[nElements];
   }
   // Wrapper around operator delete
   static void delete_Calib(void *p) {
      delete (static_cast<::Calib*>(p));
   }
   static void deleteArray_Calib(void *p) {
      delete [] (static_cast<::Calib*>(p));
   }
   static void destruct_Calib(void *p) {
      typedef ::Calib current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Calib

//______________________________________________________________________________
void Tile::Streamer(TBuffer &R__b)
{
   // Stream an object of class Tile.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Tile::Class(),this);
   } else {
      R__b.WriteClassBuffer(Tile::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Tile(void *p) {
      return  p ? new(p) ::Tile : new ::Tile;
   }
   static void *newArray_Tile(Long_t nElements, void *p) {
      return p ? new(p) ::Tile[nElements] : new ::Tile[nElements];
   }
   // Wrapper around operator delete
   static void delete_Tile(void *p) {
      delete (static_cast<::Tile*>(p));
   }
   static void deleteArray_Tile(void *p) {
      delete [] (static_cast<::Tile*>(p));
   }
   static void destruct_Tile(void *p) {
      typedef ::Tile current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Tile

//______________________________________________________________________________
void Caen::Streamer(TBuffer &R__b)
{
   // Stream an object of class Caen.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Caen::Class(),this);
   } else {
      R__b.WriteClassBuffer(Caen::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Caen(void *p) {
      return  p ? new(p) ::Caen : new ::Caen;
   }
   static void *newArray_Caen(Long_t nElements, void *p) {
      return p ? new(p) ::Caen[nElements] : new ::Caen[nElements];
   }
   // Wrapper around operator delete
   static void delete_Caen(void *p) {
      delete (static_cast<::Caen*>(p));
   }
   static void deleteArray_Caen(void *p) {
      delete [] (static_cast<::Caen*>(p));
   }
   static void destruct_Caen(void *p) {
      typedef ::Caen current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Caen

//______________________________________________________________________________
void Hgcroc::Streamer(TBuffer &R__b)
{
   // Stream an object of class Hgcroc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Hgcroc::Class(),this);
   } else {
      R__b.WriteClassBuffer(Hgcroc::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Hgcroc(void *p) {
      return  p ? new(p) ::Hgcroc : new ::Hgcroc;
   }
   static void *newArray_Hgcroc(Long_t nElements, void *p) {
      return p ? new(p) ::Hgcroc[nElements] : new ::Hgcroc[nElements];
   }
   // Wrapper around operator delete
   static void delete_Hgcroc(void *p) {
      delete (static_cast<::Hgcroc*>(p));
   }
   static void deleteArray_Hgcroc(void *p) {
      delete [] (static_cast<::Hgcroc*>(p));
   }
   static void destruct_Hgcroc(void *p) {
      typedef ::Hgcroc current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Hgcroc

namespace ROOT {
   // Wrappers around operator new
   static void *new_Event(void *p) {
      return  p ? new(p) ::Event : new ::Event;
   }
   static void *newArray_Event(Long_t nElements, void *p) {
      return p ? new(p) ::Event[nElements] : new ::Event[nElements];
   }
   // Wrapper around operator delete
   static void delete_Event(void *p) {
      delete (static_cast<::Event*>(p));
   }
   static void deleteArray_Event(void *p) {
      delete [] (static_cast<::Event*>(p));
   }
   static void destruct_Event(void *p) {
      typedef ::Event current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::Event

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = nullptr);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 389,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<int>","std::vector<int, std::allocator<int> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<int>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<int>*>(nullptr))->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete (static_cast<vector<int>*>(p));
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] (static_cast<vector<int>*>(p));
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlETilegR_Dictionary();
   static void vectorlETilegR_TClassManip(TClass*);
   static void *new_vectorlETilegR(void *p = nullptr);
   static void *newArray_vectorlETilegR(Long_t size, void *p);
   static void delete_vectorlETilegR(void *p);
   static void deleteArray_vectorlETilegR(void *p);
   static void destruct_vectorlETilegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Tile>*)
   {
      vector<Tile> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Tile>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Tile>", -2, "vector", 389,
                  typeid(vector<Tile>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETilegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<Tile>) );
      instance.SetNew(&new_vectorlETilegR);
      instance.SetNewArray(&newArray_vectorlETilegR);
      instance.SetDelete(&delete_vectorlETilegR);
      instance.SetDeleteArray(&deleteArray_vectorlETilegR);
      instance.SetDestructor(&destruct_vectorlETilegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Tile> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<Tile>","std::vector<Tile, std::allocator<Tile> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<Tile>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETilegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<Tile>*>(nullptr))->GetClass();
      vectorlETilegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETilegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETilegR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Tile> : new vector<Tile>;
   }
   static void *newArray_vectorlETilegR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Tile>[nElements] : new vector<Tile>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETilegR(void *p) {
      delete (static_cast<vector<Tile>*>(p));
   }
   static void deleteArray_vectorlETilegR(void *p) {
      delete [] (static_cast<vector<Tile>*>(p));
   }
   static void destruct_vectorlETilegR(void *p) {
      typedef vector<Tile> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<Tile>

namespace ROOT {
   static TClass *vectorlEHgcrocgR_Dictionary();
   static void vectorlEHgcrocgR_TClassManip(TClass*);
   static void *new_vectorlEHgcrocgR(void *p = nullptr);
   static void *newArray_vectorlEHgcrocgR(Long_t size, void *p);
   static void delete_vectorlEHgcrocgR(void *p);
   static void deleteArray_vectorlEHgcrocgR(void *p);
   static void destruct_vectorlEHgcrocgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Hgcroc>*)
   {
      vector<Hgcroc> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Hgcroc>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Hgcroc>", -2, "vector", 389,
                  typeid(vector<Hgcroc>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEHgcrocgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<Hgcroc>) );
      instance.SetNew(&new_vectorlEHgcrocgR);
      instance.SetNewArray(&newArray_vectorlEHgcrocgR);
      instance.SetDelete(&delete_vectorlEHgcrocgR);
      instance.SetDeleteArray(&deleteArray_vectorlEHgcrocgR);
      instance.SetDestructor(&destruct_vectorlEHgcrocgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Hgcroc> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<Hgcroc>","std::vector<Hgcroc, std::allocator<Hgcroc> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<Hgcroc>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEHgcrocgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<Hgcroc>*>(nullptr))->GetClass();
      vectorlEHgcrocgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEHgcrocgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEHgcrocgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Hgcroc> : new vector<Hgcroc>;
   }
   static void *newArray_vectorlEHgcrocgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Hgcroc>[nElements] : new vector<Hgcroc>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEHgcrocgR(void *p) {
      delete (static_cast<vector<Hgcroc>*>(p));
   }
   static void deleteArray_vectorlEHgcrocgR(void *p) {
      delete [] (static_cast<vector<Hgcroc>*>(p));
   }
   static void destruct_vectorlEHgcrocgR(void *p) {
      typedef vector<Hgcroc> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<Hgcroc>

namespace ROOT {
   static TClass *vectorlECaengR_Dictionary();
   static void vectorlECaengR_TClassManip(TClass*);
   static void *new_vectorlECaengR(void *p = nullptr);
   static void *newArray_vectorlECaengR(Long_t size, void *p);
   static void delete_vectorlECaengR(void *p);
   static void deleteArray_vectorlECaengR(void *p);
   static void destruct_vectorlECaengR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Caen>*)
   {
      vector<Caen> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Caen>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Caen>", -2, "vector", 389,
                  typeid(vector<Caen>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECaengR_Dictionary, isa_proxy, 4,
                  sizeof(vector<Caen>) );
      instance.SetNew(&new_vectorlECaengR);
      instance.SetNewArray(&newArray_vectorlECaengR);
      instance.SetDelete(&delete_vectorlECaengR);
      instance.SetDeleteArray(&deleteArray_vectorlECaengR);
      instance.SetDestructor(&destruct_vectorlECaengR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Caen> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<Caen>","std::vector<Caen, std::allocator<Caen> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<Caen>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECaengR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<Caen>*>(nullptr))->GetClass();
      vectorlECaengR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECaengR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECaengR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Caen> : new vector<Caen>;
   }
   static void *newArray_vectorlECaengR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<Caen>[nElements] : new vector<Caen>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECaengR(void *p) {
      delete (static_cast<vector<Caen>*>(p));
   }
   static void deleteArray_vectorlECaengR(void *p) {
      delete [] (static_cast<vector<Caen>*>(p));
   }
   static void destruct_vectorlECaengR(void *p) {
      typedef vector<Caen> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<Caen>

namespace ROOT {
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary();
   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOintgRcOintgR(void *p = nullptr);
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOintgRcOintgR(void *p);
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p);
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,int>,int>*)
   {
      map<pair<int,int>,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,int>,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,int>,int>", -2, "map", 100,
                  typeid(map<pair<int,int>,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOintgRcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<pair<int,int>,int>) );
      instance.SetNew(&new_maplEpairlEintcOintgRcOintgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDelete(&delete_maplEpairlEintcOintgRcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOintgRcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,int>,int> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<pair<int,int>,int>","std::map<std::pair<int, int>, int, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, int> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<pair<int,int>,int>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<pair<int,int>,int>*>(nullptr))->GetClass();
      maplEpairlEintcOintgRcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOintgRcOintgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<pair<int,int>,int> : new map<pair<int,int>,int>;
   }
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<pair<int,int>,int>[nElements] : new map<pair<int,int>,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOintgRcOintgR(void *p) {
      delete (static_cast<map<pair<int,int>,int>*>(p));
   }
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p) {
      delete [] (static_cast<map<pair<int,int>,int>*>(p));
   }
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p) {
      typedef map<pair<int,int>,int> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,int>,int>

namespace ROOT {
   static TClass *maplEintcOintgR_Dictionary();
   static void maplEintcOintgR_TClassManip(TClass*);
   static void *new_maplEintcOintgR(void *p = nullptr);
   static void *newArray_maplEintcOintgR(Long_t size, void *p);
   static void delete_maplEintcOintgR(void *p);
   static void deleteArray_maplEintcOintgR(void *p);
   static void destruct_maplEintcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,int>*)
   {
      map<int,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,int>", -2, "map", 100,
                  typeid(map<int,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,int>) );
      instance.SetNew(&new_maplEintcOintgR);
      instance.SetNewArray(&newArray_maplEintcOintgR);
      instance.SetDelete(&delete_maplEintcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOintgR);
      instance.SetDestructor(&destruct_maplEintcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,int> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<int,int>","std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,int>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,int>*>(nullptr))->GetClass();
      maplEintcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOintgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,int> : new map<int,int>;
   }
   static void *newArray_maplEintcOintgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,int>[nElements] : new map<int,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOintgR(void *p) {
      delete (static_cast<map<int,int>*>(p));
   }
   static void deleteArray_maplEintcOintgR(void *p) {
      delete [] (static_cast<map<int,int>*>(p));
   }
   static void destruct_maplEintcOintgR(void *p) {
      typedef map<int,int> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,int>

namespace ROOT {
   static TClass *maplEintcOdoublegR_Dictionary();
   static void maplEintcOdoublegR_TClassManip(TClass*);
   static void *new_maplEintcOdoublegR(void *p = nullptr);
   static void *newArray_maplEintcOdoublegR(Long_t size, void *p);
   static void delete_maplEintcOdoublegR(void *p);
   static void deleteArray_maplEintcOdoublegR(void *p);
   static void destruct_maplEintcOdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,double>*)
   {
      map<int,double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,double>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,double>", -2, "map", 100,
                  typeid(map<int,double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,double>) );
      instance.SetNew(&new_maplEintcOdoublegR);
      instance.SetNewArray(&newArray_maplEintcOdoublegR);
      instance.SetDelete(&delete_maplEintcOdoublegR);
      instance.SetDeleteArray(&deleteArray_maplEintcOdoublegR);
      instance.SetDestructor(&destruct_maplEintcOdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,double> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<int,double>","std::map<int, double, std::less<int>, std::allocator<std::pair<int const, double> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,double>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,double>*>(nullptr))->GetClass();
      maplEintcOdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOdoublegR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,double> : new map<int,double>;
   }
   static void *newArray_maplEintcOdoublegR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,double>[nElements] : new map<int,double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOdoublegR(void *p) {
      delete (static_cast<map<int,double>*>(p));
   }
   static void deleteArray_maplEintcOdoublegR(void *p) {
      delete [] (static_cast<map<int,double>*>(p));
   }
   static void destruct_maplEintcOdoublegR(void *p) {
      typedef map<int,double> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,double>

namespace ROOT {
   static TClass *maplEintcOTilegR_Dictionary();
   static void maplEintcOTilegR_TClassManip(TClass*);
   static void *new_maplEintcOTilegR(void *p = nullptr);
   static void *newArray_maplEintcOTilegR(Long_t size, void *p);
   static void delete_maplEintcOTilegR(void *p);
   static void deleteArray_maplEintcOTilegR(void *p);
   static void destruct_maplEintcOTilegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,Tile>*)
   {
      map<int,Tile> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,Tile>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,Tile>", -2, "map", 100,
                  typeid(map<int,Tile>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTilegR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,Tile>) );
      instance.SetNew(&new_maplEintcOTilegR);
      instance.SetNewArray(&newArray_maplEintcOTilegR);
      instance.SetDelete(&delete_maplEintcOTilegR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTilegR);
      instance.SetDestructor(&destruct_maplEintcOTilegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,Tile> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<int,Tile>","std::map<int, Tile, std::less<int>, std::allocator<std::pair<int const, Tile> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,Tile>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTilegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,Tile>*>(nullptr))->GetClass();
      maplEintcOTilegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTilegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTilegR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Tile> : new map<int,Tile>;
   }
   static void *newArray_maplEintcOTilegR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Tile>[nElements] : new map<int,Tile>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTilegR(void *p) {
      delete (static_cast<map<int,Tile>*>(p));
   }
   static void deleteArray_maplEintcOTilegR(void *p) {
      delete [] (static_cast<map<int,Tile>*>(p));
   }
   static void destruct_maplEintcOTilegR(void *p) {
      typedef map<int,Tile> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,Tile>

namespace ROOT {
   static TClass *maplEintcOTilemUgR_Dictionary();
   static void maplEintcOTilemUgR_TClassManip(TClass*);
   static void *new_maplEintcOTilemUgR(void *p = nullptr);
   static void *newArray_maplEintcOTilemUgR(Long_t size, void *p);
   static void delete_maplEintcOTilemUgR(void *p);
   static void deleteArray_maplEintcOTilemUgR(void *p);
   static void destruct_maplEintcOTilemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,Tile*>*)
   {
      map<int,Tile*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,Tile*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,Tile*>", -2, "map", 100,
                  typeid(map<int,Tile*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTilemUgR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,Tile*>) );
      instance.SetNew(&new_maplEintcOTilemUgR);
      instance.SetNewArray(&newArray_maplEintcOTilemUgR);
      instance.SetDelete(&delete_maplEintcOTilemUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTilemUgR);
      instance.SetDestructor(&destruct_maplEintcOTilemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,Tile*> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<int,Tile*>","std::map<int, Tile*, std::less<int>, std::allocator<std::pair<int const, Tile*> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,Tile*>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTilemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,Tile*>*>(nullptr))->GetClass();
      maplEintcOTilemUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTilemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTilemUgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Tile*> : new map<int,Tile*>;
   }
   static void *newArray_maplEintcOTilemUgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Tile*>[nElements] : new map<int,Tile*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTilemUgR(void *p) {
      delete (static_cast<map<int,Tile*>*>(p));
   }
   static void deleteArray_maplEintcOTilemUgR(void *p) {
      delete [] (static_cast<map<int,Tile*>*>(p));
   }
   static void destruct_maplEintcOTilemUgR(void *p) {
      typedef map<int,Tile*> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,Tile*>

namespace ROOT {
   static TClass *maplEintcOTStringgR_Dictionary();
   static void maplEintcOTStringgR_TClassManip(TClass*);
   static void *new_maplEintcOTStringgR(void *p = nullptr);
   static void *newArray_maplEintcOTStringgR(Long_t size, void *p);
   static void delete_maplEintcOTStringgR(void *p);
   static void deleteArray_maplEintcOTStringgR(void *p);
   static void destruct_maplEintcOTStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,TString>*)
   {
      map<int,TString> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,TString>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,TString>", -2, "map", 100,
                  typeid(map<int,TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOTStringgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,TString>) );
      instance.SetNew(&new_maplEintcOTStringgR);
      instance.SetNewArray(&newArray_maplEintcOTStringgR);
      instance.SetDelete(&delete_maplEintcOTStringgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOTStringgR);
      instance.SetDestructor(&destruct_maplEintcOTStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,TString> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<int,TString>","std::map<int, TString, std::less<int>, std::allocator<std::pair<int const, TString> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,TString>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOTStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,TString>*>(nullptr))->GetClass();
      maplEintcOTStringgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOTStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOTStringgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,TString> : new map<int,TString>;
   }
   static void *newArray_maplEintcOTStringgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,TString>[nElements] : new map<int,TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOTStringgR(void *p) {
      delete (static_cast<map<int,TString>*>(p));
   }
   static void deleteArray_maplEintcOTStringgR(void *p) {
      delete [] (static_cast<map<int,TString>*>(p));
   }
   static void destruct_maplEintcOTStringgR(void *p) {
      typedef map<int,TString> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,TString>

namespace ROOT {
   static TClass *maplEintcOHgcrocgR_Dictionary();
   static void maplEintcOHgcrocgR_TClassManip(TClass*);
   static void *new_maplEintcOHgcrocgR(void *p = nullptr);
   static void *newArray_maplEintcOHgcrocgR(Long_t size, void *p);
   static void delete_maplEintcOHgcrocgR(void *p);
   static void deleteArray_maplEintcOHgcrocgR(void *p);
   static void destruct_maplEintcOHgcrocgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,Hgcroc>*)
   {
      map<int,Hgcroc> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,Hgcroc>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,Hgcroc>", -2, "map", 100,
                  typeid(map<int,Hgcroc>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOHgcrocgR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,Hgcroc>) );
      instance.SetNew(&new_maplEintcOHgcrocgR);
      instance.SetNewArray(&newArray_maplEintcOHgcrocgR);
      instance.SetDelete(&delete_maplEintcOHgcrocgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOHgcrocgR);
      instance.SetDestructor(&destruct_maplEintcOHgcrocgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,Hgcroc> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<int,Hgcroc>","std::map<int, Hgcroc, std::less<int>, std::allocator<std::pair<int const, Hgcroc> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,Hgcroc>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOHgcrocgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,Hgcroc>*>(nullptr))->GetClass();
      maplEintcOHgcrocgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOHgcrocgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOHgcrocgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Hgcroc> : new map<int,Hgcroc>;
   }
   static void *newArray_maplEintcOHgcrocgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Hgcroc>[nElements] : new map<int,Hgcroc>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOHgcrocgR(void *p) {
      delete (static_cast<map<int,Hgcroc>*>(p));
   }
   static void deleteArray_maplEintcOHgcrocgR(void *p) {
      delete [] (static_cast<map<int,Hgcroc>*>(p));
   }
   static void destruct_maplEintcOHgcrocgR(void *p) {
      typedef map<int,Hgcroc> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,Hgcroc>

namespace ROOT {
   static TClass *maplEintcOHgcrocmUgR_Dictionary();
   static void maplEintcOHgcrocmUgR_TClassManip(TClass*);
   static void *new_maplEintcOHgcrocmUgR(void *p = nullptr);
   static void *newArray_maplEintcOHgcrocmUgR(Long_t size, void *p);
   static void delete_maplEintcOHgcrocmUgR(void *p);
   static void deleteArray_maplEintcOHgcrocmUgR(void *p);
   static void destruct_maplEintcOHgcrocmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,Hgcroc*>*)
   {
      map<int,Hgcroc*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,Hgcroc*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,Hgcroc*>", -2, "map", 100,
                  typeid(map<int,Hgcroc*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOHgcrocmUgR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,Hgcroc*>) );
      instance.SetNew(&new_maplEintcOHgcrocmUgR);
      instance.SetNewArray(&newArray_maplEintcOHgcrocmUgR);
      instance.SetDelete(&delete_maplEintcOHgcrocmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOHgcrocmUgR);
      instance.SetDestructor(&destruct_maplEintcOHgcrocmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,Hgcroc*> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<int,Hgcroc*>","std::map<int, Hgcroc*, std::less<int>, std::allocator<std::pair<int const, Hgcroc*> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,Hgcroc*>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOHgcrocmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,Hgcroc*>*>(nullptr))->GetClass();
      maplEintcOHgcrocmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOHgcrocmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOHgcrocmUgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Hgcroc*> : new map<int,Hgcroc*>;
   }
   static void *newArray_maplEintcOHgcrocmUgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Hgcroc*>[nElements] : new map<int,Hgcroc*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOHgcrocmUgR(void *p) {
      delete (static_cast<map<int,Hgcroc*>*>(p));
   }
   static void deleteArray_maplEintcOHgcrocmUgR(void *p) {
      delete [] (static_cast<map<int,Hgcroc*>*>(p));
   }
   static void destruct_maplEintcOHgcrocmUgR(void *p) {
      typedef map<int,Hgcroc*> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,Hgcroc*>

namespace ROOT {
   static TClass *maplEintcOCaengR_Dictionary();
   static void maplEintcOCaengR_TClassManip(TClass*);
   static void *new_maplEintcOCaengR(void *p = nullptr);
   static void *newArray_maplEintcOCaengR(Long_t size, void *p);
   static void delete_maplEintcOCaengR(void *p);
   static void deleteArray_maplEintcOCaengR(void *p);
   static void destruct_maplEintcOCaengR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,Caen>*)
   {
      map<int,Caen> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,Caen>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,Caen>", -2, "map", 100,
                  typeid(map<int,Caen>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCaengR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,Caen>) );
      instance.SetNew(&new_maplEintcOCaengR);
      instance.SetNewArray(&newArray_maplEintcOCaengR);
      instance.SetDelete(&delete_maplEintcOCaengR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCaengR);
      instance.SetDestructor(&destruct_maplEintcOCaengR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,Caen> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<int,Caen>","std::map<int, Caen, std::less<int>, std::allocator<std::pair<int const, Caen> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,Caen>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCaengR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,Caen>*>(nullptr))->GetClass();
      maplEintcOCaengR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCaengR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCaengR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Caen> : new map<int,Caen>;
   }
   static void *newArray_maplEintcOCaengR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Caen>[nElements] : new map<int,Caen>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCaengR(void *p) {
      delete (static_cast<map<int,Caen>*>(p));
   }
   static void deleteArray_maplEintcOCaengR(void *p) {
      delete [] (static_cast<map<int,Caen>*>(p));
   }
   static void destruct_maplEintcOCaengR(void *p) {
      typedef map<int,Caen> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,Caen>

namespace ROOT {
   static TClass *maplEintcOCaenmUgR_Dictionary();
   static void maplEintcOCaenmUgR_TClassManip(TClass*);
   static void *new_maplEintcOCaenmUgR(void *p = nullptr);
   static void *newArray_maplEintcOCaenmUgR(Long_t size, void *p);
   static void delete_maplEintcOCaenmUgR(void *p);
   static void deleteArray_maplEintcOCaenmUgR(void *p);
   static void destruct_maplEintcOCaenmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,Caen*>*)
   {
      map<int,Caen*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,Caen*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,Caen*>", -2, "map", 100,
                  typeid(map<int,Caen*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOCaenmUgR_Dictionary, isa_proxy, 4,
                  sizeof(map<int,Caen*>) );
      instance.SetNew(&new_maplEintcOCaenmUgR);
      instance.SetNewArray(&newArray_maplEintcOCaenmUgR);
      instance.SetDelete(&delete_maplEintcOCaenmUgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOCaenmUgR);
      instance.SetDestructor(&destruct_maplEintcOCaenmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,Caen*> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("map<int,Caen*>","std::map<int, Caen*, std::less<int>, std::allocator<std::pair<int const, Caen*> > >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const map<int,Caen*>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOCaenmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const map<int,Caen*>*>(nullptr))->GetClass();
      maplEintcOCaenmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOCaenmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOCaenmUgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Caen*> : new map<int,Caen*>;
   }
   static void *newArray_maplEintcOCaenmUgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) map<int,Caen*>[nElements] : new map<int,Caen*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOCaenmUgR(void *p) {
      delete (static_cast<map<int,Caen*>*>(p));
   }
   static void deleteArray_maplEintcOCaenmUgR(void *p) {
      delete [] (static_cast<map<int,Caen*>*>(p));
   }
   static void destruct_maplEintcOCaenmUgR(void *p) {
      typedef map<int,Caen*> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class map<int,Caen*>

namespace {
  void TriggerDictionaryInitialization_Cint_Impl() {
    static const char* headers[] = {
"RootSetupWrapper.h",
"Calib.h",
"Setup.h",
"Event.h",
"Tile.h",
"HGCROC.h",
"Caen.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/vandrieu/Software/Root/Myinstall/include/",
"/home/vandrieu/Software/epiclfhcal/NewStructure/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Cint dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$Caen.h")))  __attribute__((annotate("$clingAutoload$Event.h")))  Caen;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$HGCROC.h")))  __attribute__((annotate("$clingAutoload$Event.h")))  Hgcroc;
class __attribute__((annotate("$clingAutoload$Tile.h")))  __attribute__((annotate("$clingAutoload$Event.h")))  Tile;
namespace std{template <typename _Tp = void> struct __attribute__((annotate("$clingAutoload$bits/stl_function.h")))  __attribute__((annotate("$clingAutoload$string")))  less;
}
namespace std{template <typename _T1, typename _T2> struct __attribute__((annotate("$clingAutoload$bits/stl_iterator.h")))  __attribute__((annotate("$clingAutoload$string")))  pair;
}
class __attribute__((annotate("$clingAutoload$RootSetupWrapper.h")))  RootSetupWrapper;
class __attribute__((annotate("$clingAutoload$Setup.h")))  __attribute__((annotate("$clingAutoload$Calib.h")))  Setup;
class __attribute__((annotate("$clingAutoload$Calib.h")))  Calib;
class __attribute__((annotate("$clingAutoload$Event.h")))  Event;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Cint dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RootSetupWrapper.h"
#include "Calib.h"
#include "Setup.h"
#include "Event.h"
#include "Tile.h"
#include "HGCROC.h"
#include "Caen.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"Caen", payloadCode, "@",
"Calib", payloadCode, "@",
"Event", payloadCode, "@",
"Hgcroc", payloadCode, "@",
"RootSetupWrapper", payloadCode, "@",
"Setup", payloadCode, "@",
"Tile", payloadCode, "@",
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
