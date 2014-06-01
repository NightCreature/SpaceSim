//#pragma once
////
////
///////////////////////////////////////////////////////////////
////// Termination of Link List
///////////////////////////////////////////////////////////////
//struct End
//{
//};
//
/////////////////////////////////////////////////////////////
//// Node of Static Link List
/////////////////////////////////////////////////////////////
//template <int iData, typename Type>
//struct Node
//{
//    enum { value = iData };
//    typedef Type Next;
//};
//
/////////////////////////////////////////////////////////////
//// Insert item into Static Link List
/////////////////////////////////////////////////////////////
//template <int iData, typename Type>
//struct PushBack
//{
//    typedef Node<iData, Type> staticList;
//};
//
//template <typename T>
//struct SizeT;
//
//template <int iData, typename Type>
//struct SizeT<Node<iData, Type> >
//{
//    enum { value = 1 + SizeT<Type>::value };
//};
//
//template <>
//struct SizeT<End>
//{
//    enum { value = 0 };
//};
//
//struct FNVHashOffset
//{
//    enum { value = 2166136261 };
//};
//
//struct FNVHashPrime
//{
//    enum { value = 16777619};
//};
//
//template<int iData, typename T>
//struct HashT;
//
//template<int iData, typename T>
//struct HashT<iData, Node<iData, T> >
//{
//    enum { value = (HashT<T::value, T::Next>::value ^ iData) * FNVHashPrime::value };
//};
//
//template<int iData>
//struct HashT<iData, Node<iData, End> >
//{
//    enum { value = (iData ^ FNVHashOffset::value) * FNVHashPrime };
//};
//
//template <int iIndex, typename T, int iStart = 0>
//struct At;
//
//template <int iIndex, int iData, typename Type, int iStart>
//struct At<iIndex, Node<iData, Type>, iStart>
//{
//    enum { value = iIndex == iStart ? 
//           iData : At<iIndex, Type, (iStart + 1)>::value };
//};
//
//template <int iIndex, int iData, int iStart>
//struct At<iIndex, Node<iData, End>, iStart>
//{
//    enum { value = iIndex == iStart ? iData : -1 };
//};
//
//
//typedef PushBack<(int)'!', End>::staticList node1;
//typedef PushBack<(int)'0', node1>::staticList node2;
//typedef PushBack<(int)'l', node2>::staticList node3;
//typedef PushBack<(int)'l', node3>::staticList node4;
//typedef PushBack<(int)'e', node4>::staticList node5;
//typedef PushBack<(int)'H', node5>::staticList myList;