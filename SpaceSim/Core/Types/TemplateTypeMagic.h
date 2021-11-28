#pragma once

#include "Core/Macros/PlatformMacros.h"

#include <utility>
#include <array>
#include <type_traits>
#include <string>



template<class T>
struct TypeIdentity { using type = T; };

template< class ...Ts>
struct TypeList : TypeIdentity<Ts>... {};

template<class T, typename...>
struct First : TypeIdentity<T> {};

template <class ...Ts>
using Void_t = typename First<void, Ts...>::type;

template<class...>
using void_t_direct = void; //This causes issues, in specialised overload resolution in places

//Recursive search
template<class TargetT, class ...Ts>
struct recursiveIsInPack : std::false_type {}; //Target type not found

template<class TargetT, class ...Ts>
struct recursiveIsInPack<TargetT, TargetT, Ts...> : std::true_type {}; //Target type first in list

template<class TargetT, class FirstT, class ...Ts>
struct recursiveIsInPack<TargetT, FirstT, Ts...> : recursiveIsInPack<TargetT, Ts...> {}; //not found case

//linear search
template<class T>
static constexpr std::string_view nameof()
{
    return std::string_view(FUNCTION_NAME);
}

//This kind of works, at least it has a type identity we can ask a type of
template<class T, class FP>
struct FunctionPointerTypeIdentity : public TypeIdentity<T>
{
    FunctionPointerTypeIdentity(FP functionPointer) : m_functionPointer(functionPointer) {}
    FP m_functionPointer;
};

template<class T, class FP>
FunctionPointerTypeIdentity<T, FP> MakePair(FP functionPointer)
{
    return FunctionPointerTypeIdentity<T, FP>(functionPointer);
}

template<class ...Ts>
struct SearchableTypelist : TypeList<Ts...>
{
    using FunctionPointer = std::string_view(*)();
    using TypeLists = TypeList<Ts...>;
    //What you really want this to be is an array of pair that contains the FunctionPointer so we can search and the TypeIdentity of that object
    static constexpr std::array<FunctionPointer, sizeof...(Ts)> ids = { nameof<Ts>... }; //This is a hack so we can do static pointer compares in the functions below
    //            Pair arr[] = { {Orderable<Ts>::value, Is}... };
    //static constexpr std::array<FunctionPointerTypeIdentity<Ts, FunctionPointer>, sizeof...(Ts)> fpTIds = { MakePair<Ts>(nameof<Ts>...)... }; //This doesnt unwind sadly
    static constexpr bool contains(FunctionPointer Target)
    {
        return index(Target) != -1;
    }

    static constexpr size_t index(FunctionPointer Target)
    {
        for (size_t counter = 0; counter < ids.size(); ++counter)
        {
            if (ids[counter] == Target)
            {
                return counter;
            }
        }

        return static_cast<size_t>(-1);
    }

    /*static constexpr auto GetInstanceOf(Fn Target) -> decltype(type)
    {
        for (auto&& x : ids)
        {
            if (x == Target)
            {
                return type();
            }
        }
        return;
    }*/
};

//Crazy impl
//Moving the functions out of the class is faster
//
//template<class ...Ts>
//struct IsInPackImp : TypeIdentity<Ts>...
//{
//    template<class T>
//    static constexpr std::true_type check(TypeIdentity<T>) { return true; }
//    template<class T>
//    static constexpr std::false_type check(...) { return false; }
//};
//
//
//template<class TargetT, class ...Ts>
//using IsInPack = 
//    decltype(IsInPackImp<Ts...>::template 
//        check<TargetT>
//        ( IsInPack<Ts...>() )
//        );

//https://digital-medium-co-uk.zoom.us/rec/play/T-BY8Z9P5CksmBVq23kFPt2-JmUx1o5WHUb_0472GpYghYEKa8S_A2rVnNbRVFU5bYJyZI9CjQWn0OYf.6Upo9zsiOHj34X_U?continueMode=true&_x_zm_rtaid=7CCz8o15QxKEFZfjAjb0pw.1637621097487.d877717e1759973558b6039ce82c2a21&_x_zm_rhtaid=115
//55:49


//
////Sorting Types
//template<
//    template <typename> 
//        class Orderable,
//    typename ListT
//>
//class SortTypes;
//template<
//    template <typename>
//        class Orderable,
//    template <typename>
//        class List,
//    typename ...Ts>
//    class SortTypes<Orderable, List<Ts...>>
//{
//public:
//    template <typename SeqT> struct Impl;
//
//
//    template<size_t ...Is>
//    struct Impl<std::index_sequence<Is...>>
//    {
//        static constexpr std::array < size_t, sizeof...(Ts)> indexes = []
//        {
//            using Ord = std::common_type_t<decltype(Orderable<Ts>::value)...>;
//            struct Pair //Pair type we can store
//            {
//                Ord ord;
//                size_t index;
//            };
//            Pair arr[] = { {Orderable<Ts>::value, Is}... };
//            //Insertion sort here
//            std::sort(std::begin(arr), std::end(arr), [](auto x, auto y)
//                {
//                    return x.ord < y.ord;
//                });
//            return std::array<size_t, sizeof...(Ts)>{arr[Is].index...};
//        }();
//        using type = List<std::tuple_element_t<indexes[Is], TypeList<Ts...>>...>;
//    };
//
//    using type = typename Impl<std::make_index_sequence<sizeof...(Ts)>>::type;
//};
//
//template<class T>
//using Orderable = std::integral_constant<size_t, alignof(T)>;
//
//using Sorted = typename SortTypes< Orderable, TypeList<int, double, unsigned, char>>::type;
//
////This is abusing the fact that we can create a static member variable and that the pointer to that variable is the same in all instances
//class TypeId
//{
//    static constexpr std::string_view m_empty{}; //Abuse of static type members with the next line to do RTTI matching
//    const std::string_view* m_sv = &m_empty;
//
//    template<class T> friend constexpr TypeId type_id();
//
//    friend constexpr bool operator== (TypeId lhs, TypeId rhs) { return lhs.m_sv == rhs.m_sv; }
//    friend constexpr bool operator< (TypeId lhs, TypeId rhs) { return lhs.m_sv < rhs.m_sv; }
//};
//
//template<class T>
//struct TypeOrderable
//{
//    static constexpr TypeId value = type_id<T>();
//};
//
//template< template <class ...> class List,
//    class ...Ts>
//using Normalized = typename SortTypes<TypeOrderable, List<Ts...>>::type;
//
