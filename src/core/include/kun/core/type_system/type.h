#pragma once

// 通过一个全局唯一的type对象来标识类型
// Type类作为接口类使用, 给出一个类型最粗略的认知
//      - align, size, raw_category: 基础的内存描述, 以及一个粗略的类型来标识
//      - custom_cate: 为其他系统提供自定义的大类cate
//      - func table: 基础功能描述, ctor/dtor, 二段式构造, 比较, 四则运算, copy/move, 序列化
// 对于容器/Object, 我们提供继承的实现, 对于万一有custom类型的需求, 我们提供的custom_id可以作为转换的检测
// 应用场景
//      - any(blackboard/port): 主要用作为类型标识
//      - mesh property: 主要使用序列化, 四则运算, 比较
//      - node registry: 主要使用类型标识
//      - cross language: 主要使用了类型标识, 并反向提供功能
// 使用模板来get任意类型的Type
//      - 对于基础类型, 使用全特化的方式提供实现
//      - 对于object, 可以使用宏以侵入的方式提供实现
//      - 提供一个返回指针引用的template作为基础, 通过模板get的时候检查template是否实现

namespace kun
{
enum class ETypeCate
{
    Basic,   // cpp basic type
    Array,   // array container
    UMap,    // map container
    USet,    // set container
    Object,  // object
    UserType,// user def
};

}