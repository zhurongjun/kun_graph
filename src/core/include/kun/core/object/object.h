#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/type_system/cast.h"
#include "kun/core/functional/flag_tools.hpp"
#include "kun/core/std/kstl/name.h"

// object def
// life circle
//    Object()                             [flag: OnConstruct]
//    ...custom data, like serialize etc.  [flag: OnInit]
//    init()                               [flag: OnInit]
//
//    ...use as normal cpp object          [flag: Complete]
//
//    shutdown()                           [flag: OnShutdown]
//    ~Object()
//
// why 2-step construction
//    we need know some data when we do logic initialize, if we use 1-step construction,
//    we can't do data setting and serialize before logic initialize. usually,
// what should we do in cpp constructor?
//    only do default data initialize or simple operations, and never throw
// what should we do in init()
//    do logic initialize, io, object create etc...
//
// why 2-step destruction
//    1. we can't control cpp destruction exec flow,
//    2. we can't call virtual function in cpp destruction
//    3. cpp destruction is force [noexcept]
//    4. we may want to reuse some object by call init() again
// what should we do in shutdown()
//    1. break object link(must first step, if we do it later, other object may get an incomplete object)
//    2. free temp memory, destroy objects etc...
//    3. reset data for reuse
// what should we do in cpp destructor?
//    destroy public data, usually noting to do
namespace kun
{
class Archive;
using ObjectFlag = u32;

class KUN_CORE_API Object
{
public:
    Object();
    virtual ~Object() = default;

    // init & shutdown
    void init();
    void shutdown();

    // serialize
    void serialize(Archive& ar);

    // flag
    ObjectFlag objFlag() const;
    bool allObjFlag(ObjectFlag flag) const;
    bool anyObjFlag(ObjectFlag flag) const;
    void setObjFlag(ObjectFlag flag);
    void eraseObjFlag(ObjectFlag flag);

    // cast
    template<class T> T as() const;
    template<class T> bool is() const;

    // object tree
    Object* parent() const;
    void setParent(Object* parent);
    template<class T> T parent() const;
    template<class T> T nearestParent() const;
    template<class T> T farthestParent() const;
    template<class T, typename TFunc> void eachParent(TFunc&& func) const noexcept;
    bool isChildOf(const Object* parent) const noexcept;
    bool isParentOf(const Object* child) const noexcept;

    // object name
    virtual Name name() { return {}; }
    virtual void rename(Name name) {}

protected:
    // !!!!!Begin Object API
    virtual void onInit() {}
    virtual void onShutdown() {}
    virtual void onSerialize(Archive& ar) {}
    // !!!!!End Object API

private:
    Object* m_parent; // 64 bit
    ObjectFlag m_flag;// 32 bit
};

}// namespace kun

// object flag impl
namespace kun
{
// impl object flag
enum ObjectFlag_
{
    ObjectFlag_None = 0,

    // life circle
    ObjectFlag_OnConstruct = 1 << 0,
    ObjectFlag_OnInit = 1 << 1,
    ObjectFlag_OnShutdown = 1 << 2,
    ObjectFlag_OnSerialize = 1 << 3,

    // object state
    ObjectFlag_Complete = 1 << 4,
};
}// namespace kun

// object RAII tools
namespace kun
{
class ObjSetFlag
{
public:
    KUN_INLINE ObjSetFlag(Object* obj, ObjectFlag flag_to_set)
        : m_obj(obj)
        , m_flag_to_set(flag_to_set)
        , m_resume_flag(ObjectFlag_None)
    {
    }

    KUN_INLINE void lock()
    {
        m_resume_flag = (~m_obj->objFlag()) & m_flag_to_set;
        m_obj->setObjFlag(m_flag_to_set);
    }
    KUN_INLINE void unlock() { m_obj->eraseObjFlag(m_resume_flag); }

private:
    Object* m_obj;
    ObjectFlag m_flag_to_set;
    ObjectFlag m_resume_flag;
};
class ObjEraseFlag
{
    KUN_INLINE ObjEraseFlag(Object* obj, ObjectFlag flag_to_erase)
        : m_obj(obj)
        , m_flag_to_erase(flag_to_erase)
        , m_resume_flag(ObjectFlag_None)

    {
    }

    KUN_INLINE void lock()
    {
        m_resume_flag = m_obj->objFlag() & m_flag_to_erase;
        m_obj->eraseObjFlag(m_flag_to_erase);
    }
    KUN_INLINE void unlock() { m_obj->setObjFlag(m_resume_flag); }

private:
    Object* m_obj;
    ObjectFlag m_flag_to_erase;
    ObjectFlag m_resume_flag;
};
}// namespace kun

// object impl
namespace kun
{
// ctor & dtor
KUN_INLINE Object::Object()
    : m_parent(nullptr)
    , m_flag(ObjectFlag_OnConstruct)
{
}

// init & shutdown
KUN_INLINE void Object::init()
{
    if (!allObjFlag(ObjectFlag_Complete))
    {
        onInit();
        eraseObjFlag(ObjectFlag_OnInit);

        setObjFlag(ObjectFlag_Complete);
    }
}
KUN_INLINE void Object::shutdown()
{
    if (allObjFlag(ObjectFlag_Complete))
    {
        {
            RAII raii(ObjSetFlag(this, ObjectFlag_OnShutdown));
            onShutdown();
        }
        eraseObjFlag(ObjectFlag_Complete);
    }
}

// serialize
KUN_INLINE void Object::serialize(Archive& ar)
{
    RAII raii(ObjSetFlag(this, ObjectFlag_OnSerialize));
    onSerialize(ar);
}

// flag
KUN_INLINE ObjectFlag Object::objFlag() const { return m_flag; }
KUN_INLINE bool Object::allObjFlag(ObjectFlag flag) const { return allFlag(m_flag, flag); }
KUN_INLINE bool Object::anyObjFlag(ObjectFlag flag) const { return anyFlag(m_flag, flag); }
KUN_INLINE void Object::setObjFlag(ObjectFlag flag) { m_flag = setFlag(m_flag, flag); }
KUN_INLINE void Object::eraseObjFlag(ObjectFlag flag) { m_flag = eraseFlag(m_flag, flag); }

// cast
template<class T> KUN_INLINE T Object::as() const { return cast<T>(const_cast<Object*>(this)); }
template<class T> KUN_INLINE bool Object::is() const { return as<T>() != nullptr; }

// object tree
KUN_INLINE Object* Object::parent() const { return m_parent; }
KUN_INLINE void Object::setParent(Object* parent) { m_parent = parent; }
template<class T> KUN_INLINE T Object::parent() const { return cast<T>(m_parent); }
template<class T> KUN_INLINE T Object::nearestParent() const
{
    for (auto o = const_cast<Object*>(this); o; o = o->parent())
    {
        if (auto parent = cast<T>(o))
        {
            return parent;
        }
    }
    return nullptr;
}
template<class T> KUN_INLINE T Object::farthestParent() const
{
    T parent = nullptr;
    for (auto o = const_cast<Object*>(this); o; o = o->parent())
    {
        if (auto p = cast<T>(o))
        {
            parent = p;
        }
    }
    return parent;
}
template<class T, typename TFunc> KUN_INLINE void Object::eachParent(TFunc&& func) const noexcept
{
    for (auto o = const_cast<Object*>(this); o; o = o->parent())
    {
        if (!func(cast<T>(o)))
            break;
    }
}
KUN_INLINE bool Object::isChildOf(const Object* parent) const noexcept
{
    for (auto o = this; o; o = o->parent())
    {
        if (o == parent)
        {
            return true;
        }
    }
    return false;
}
KUN_INLINE bool Object::isParentOf(const Object* child) const noexcept { return child->isChildOf(this); }

}// namespace kun
