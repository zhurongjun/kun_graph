#pragma once

// 通过一个全局唯一的type对象来标识类型
// Type类作为接口类使用, 给出一个类型最粗略的认知
//      - align, size, raw_category: 基础的内存描述, 以及一个粗略的类型来标识
//      - custom_id: 为其他系统提供自定义的大类id
//      - func table: 基础功能描述, ctor/dtor, 二段式构造, 比较, 四则运算, copy/move, 序列化
// 对于容器/Object, 我们提供继承的实现, 对于万一有custom类型的需求, 我们提供的custom_id可以作为转换的检测