// This version targets C++11 and later.
//
// Copyright (C) 2016-2020 Martin Moene.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// expected lite is based on:
//   A proposal to add a utility class to represent expected monad
//   by Vicente J. Botet Escriba and Pierre Talbot. http:://wg21.link/p0323

#ifndef NONSTD_EXPECTED_LITE_HPP
#define NONSTD_EXPECTED_LITE_HPP

#define expected_lite_MAJOR  0
#define expected_lite_MINOR  8
#define expected_lite_PATCH  0

#define expected_lite_VERSION  expected_STRINGIFY(expected_lite_MAJOR) "." expected_STRINGIFY(expected_lite_MINOR) "." expected_STRINGIFY(expected_lite_PATCH)

#define expected_STRINGIFY(  x )  expected_STRINGIFY_( x )
#define expected_STRINGIFY_( x )  #x


// Proposal revisions:
//
// DXXXXR0: --
// N4015  : -2 (2014-05-26)
// N4109  : -1 (2014-06-29)
// P0323R0:  0 (2016-05-28)
// P0323R1:  1 (2016-10-12)
// -------:
// P0323R2:  2 (2017-06-15)
// P0323R3:  3 (2017-10-15)
// P0323R4:  4 (2017-11-26)
// P0323R5:  5 (2018-02-08)
// P0323R6:  6 (2018-04-02)
// P0323R7:  7 (2018-06-22) *
//
// expected-lite uses 2 and higher

#ifndef  nsel_P0323R
# define nsel_P0323R  7
#endif

// Monadic operations proposal revisions:
//
// P2505R0:  0 (2021-12-12)
// P2505R1:  1 (2022-02-10)
// P2505R2:  2 (2022-04-15)
// P2505R3:  3 (2022-06-05)
// P2505R4:  4 (2022-06-15)
// P2505R5:  5 (2022-09-20) *
//
// expected-lite uses 5

#ifndef  nsel_P2505R
# define nsel_P2505R  5
#endif

// C++ language version detection (C++23 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   nsel_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define nsel_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define nsel_CPLUSPLUS  __cplusplus
# endif
#endif

#define nsel_CPP98_OR_GREATER  ( nsel_CPLUSPLUS >= 199711L )
#define nsel_CPP11_OR_GREATER  ( nsel_CPLUSPLUS >= 201103L )
#define nsel_CPP14_OR_GREATER  ( nsel_CPLUSPLUS >= 201402L )
#define nsel_CPP17_OR_GREATER  ( nsel_CPLUSPLUS >= 201703L )
#define nsel_CPP20_OR_GREATER  ( nsel_CPLUSPLUS >= 202002L )
#define nsel_CPP23_OR_GREATER  ( nsel_CPLUSPLUS >= 202300L )

#include <EASTL/utility.h>

#include <cstddef>

namespace eastl
{
//TODO: using eastl::in_place;
//TODO: using eastl::in_place_type;
//TODO: using eastl::in_place_index;
//TODO: using eastl::in_place_t;
//TODO: using eastl::in_place_type_t;
//TODO: using eastl::in_place_index_t;
// mimic templated typedef:

#define eastl_lite_in_place_t(      T)  eastl::in_place_t
#define eastl_lite_in_place_type_t( T)  eastl::in_place_type_t<T>
#define eastl_lite_in_place_index_t(K)  eastl::in_place_index_t<K>

#define eastl_lite_in_place(      T)    eastl::in_place_t{}
#define eastl_lite_in_place_type( T)    eastl::in_place_type_t<T>{}
#define eastl_lite_in_place_index(K)    eastl::in_place_index_t<K>{}
} // namespace eastl

#include <cassert>
#include <exception>
#include <EASTL/functional.h>
#include <EASTL/initializer_list.h>
#include <EASTL/memory.h>
#include <new>
#include <system_error>
#include <EASTL/type_traits.h>
#include <EASTL/utility.h>

// C++ feature usage:

#if nsel_CPP17_OR_GREATER
# define nsel_inline17 inline
#else
# define nsel_inline17 /*inline*/
#endif

// Compiler versions:
//
// MSVC++  6.0  _MSC_VER == 1200  nsel_COMPILER_MSVC_VERSION ==  60  (Visual Studio 6.0)
// MSVC++  7.0  _MSC_VER == 1300  nsel_COMPILER_MSVC_VERSION ==  70  (Visual Studio .NET 2002)
// MSVC++  7.1  _MSC_VER == 1310  nsel_COMPILER_MSVC_VERSION ==  71  (Visual Studio .NET 2003)
// MSVC++  8.0  _MSC_VER == 1400  nsel_COMPILER_MSVC_VERSION ==  80  (Visual Studio 2005)
// MSVC++  9.0  _MSC_VER == 1500  nsel_COMPILER_MSVC_VERSION ==  90  (Visual Studio 2008)
// MSVC++ 10.0  _MSC_VER == 1600  nsel_COMPILER_MSVC_VERSION == 100  (Visual Studio 2010)
// MSVC++ 11.0  _MSC_VER == 1700  nsel_COMPILER_MSVC_VERSION == 110  (Visual Studio 2012)
// MSVC++ 12.0  _MSC_VER == 1800  nsel_COMPILER_MSVC_VERSION == 120  (Visual Studio 2013)
// MSVC++ 14.0  _MSC_VER == 1900  nsel_COMPILER_MSVC_VERSION == 140  (Visual Studio 2015)
// MSVC++ 14.1  _MSC_VER >= 1910  nsel_COMPILER_MSVC_VERSION == 141  (Visual Studio 2017)
// MSVC++ 14.2  _MSC_VER >= 1920  nsel_COMPILER_MSVC_VERSION == 142  (Visual Studio 2019)

#if defined(_MSC_VER) && !defined(__clang__)
# define nsel_COMPILER_MSVC_VER      (_MSC_VER )
# define nsel_COMPILER_MSVC_VERSION  (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900)) )
#else
# define nsel_COMPILER_MSVC_VER      0
# define nsel_COMPILER_MSVC_VERSION  0
#endif

#define nsel_COMPILER_VERSION( major, minor, patch )  ( 10 * ( 10 * (major) + (minor) ) + (patch) )

#if defined(__clang__)
# define nsel_COMPILER_CLANG_VERSION  nsel_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
# define nsel_COMPILER_CLANG_VERSION  0
#endif

#if defined(__GNUC__) && !defined(__clang__)
# define nsel_COMPILER_GNUC_VERSION  nsel_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
# define nsel_COMPILER_GNUC_VERSION  0
#endif

// half-open range [lo..hi):
//#define nsel_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )

// Method enabling

#define nsel_REQUIRES_0(...) \
    template< bool B = (__VA_ARGS__), typename eastl::enable_if<B, int>::type = 0 >

#define nsel_REQUIRES_T(...) \
    , typename eastl::enable_if< (__VA_ARGS__), int >::type = 0

#define nsel_REQUIRES_R(R, ...) \
    typename eastl::enable_if< (__VA_ARGS__), R>::type

#define nsel_REQUIRES_A(...) \
    , typename eastl::enable_if< (__VA_ARGS__), void*>::type = nullptr

// Clang, GNUC, MSVC warning suppression macros:

#ifdef __clang__
# pragma clang diagnostic push
#elif defined  __GNUC__
# pragma  GCC  diagnostic push
#endif // __clang__

#if nsel_COMPILER_MSVC_VERSION >= 140
# define nsel_DISABLE_MSVC_WARNINGS(codes)  __pragma( warning(push) )  __pragma( warning(disable: codes) )
#else
# define nsel_DISABLE_MSVC_WARNINGS(codes)
#endif

#ifdef __clang__
# define nsel_RESTORE_WARNINGS()        _Pragma("clang diagnostic pop")
# define nsel_RESTORE_MSVC_WARNINGS()
#elif defined __GNUC__
# define nsel_RESTORE_WARNINGS()        _Pragma("GCC diagnostic pop")
# define nsel_RESTORE_MSVC_WARNINGS()
#elif nsel_COMPILER_MSVC_VERSION >= 140
# define nsel_RESTORE_WARNINGS()        __pragma( warning( pop ) )
# define nsel_RESTORE_MSVC_WARNINGS()   nsel_RESTORE_WARNINGS()
#else
# define nsel_RESTORE_WARNINGS()
# define nsel_RESTORE_MSVC_WARNINGS()
#endif

// Suppress the following MSVC (GSL) warnings:
// - C26409: Avoid calling new and delete explicitly, use eastl::make_unique<T> instead (r.11)

nsel_DISABLE_MSVC_WARNINGS( 26409 )

// Presence of language and library features:

#ifdef _HAS_CPP0X
# define nsel_HAS_CPP0X  _HAS_CPP0X
#else
# define nsel_HAS_CPP0X  0
#endif

// Presence of language and library features:

#define nsel_CPP17_000  (nsel_CPP17_OR_GREATER)

// Presence of C++17 language features:

#define nsel_HAVE_DEPRECATED  nsel_CPP17_000

// C++ feature usage:

#if nsel_HAVE_DEPRECATED
# define nsel_deprecated(msg) [[deprecated(msg)]]
#else
# define nsel_deprecated(msg) /*[[deprecated]]*/
#endif

//
// expected:
//

namespace eastl { namespace expected_lite {

// forward declaration:

template< typename T, typename E >
class expected;

namespace detail {

#if nsel_P2505R >= 3
template< typename T >
struct is_expected : eastl::false_type {};

template< typename T, typename E >
struct is_expected< expected< T, E > > : eastl::true_type {};
#endif // nsel_P2505R >= 3

/// discriminated union to hold value or 'error'.

template< typename T, typename E >
class storage_t_noncopy_nonmove_impl
{
    template< typename, typename > friend class eastl::expected_lite::expected;

public:
    using value_type = T;
    using error_type = E;

    // no-op construction
    storage_t_noncopy_nonmove_impl() {}
    ~storage_t_noncopy_nonmove_impl() {}

    explicit storage_t_noncopy_nonmove_impl( bool has_value )
        : m_has_value( has_value )
    {}

    void construct_value()
    {
        new( &m_value ) value_type();
    }

    // void construct_value( value_type const & e )
    // {
    //     new( &m_value ) value_type( e );
    // }

    // void construct_value( value_type && e )
    // {
    //     new( &m_value ) value_type( eastl::move( e ) );
    // }

    template< class... Args >
    void emplace_value( Args&&... args )
    {
        new( &m_value ) value_type(eastl::forward<Args>(args)...);
    }

    template< class U, class... Args >
    void emplace_value( std::initializer_list<U> il, Args&&... args )
    {
        new( &m_value ) value_type( il, eastl::forward<Args>(args)... );
    }

    void destruct_value()
    {
        m_value.~value_type();
    }

    // void construct_error( error_type const & e )
    // {
    //     // new( &m_error ) error_type( e );
    // }

    // void construct_error( error_type && e )
    // {
    //     // new( &m_error ) error_type( eastl::move( e ) );
    // }

    template< class... Args >
    void emplace_error( Args&&... args )
    {
        new( &m_error ) error_type(eastl::forward<Args>(args)...);
    }

    template< class U, class... Args >
    void emplace_error( std::initializer_list<U> il, Args&&... args )
    {
        new( &m_error ) error_type( il, eastl::forward<Args>(args)... );
    }

    void destruct_error()
    {
        m_error.~error_type();
    }

    constexpr value_type const & value() const &
    {
        return m_value;
    }

    value_type & value() &
    {
        return m_value;
    }

    constexpr value_type const && value() const &&
    {
        return eastl::move( m_value );
    }

    EA_CPP14_CONSTEXPR value_type && value() &&
    {
        return eastl::move( m_value );
    }

    value_type const * value_ptr() const
    {
        return &m_value;
    }

    value_type * value_ptr()
    {
        return &m_value;
    }

    error_type const & error() const &
    {
        return m_error;
    }

    error_type & error() &
    {
        return m_error;
    }

    constexpr error_type const && error() const &&
    {
        return eastl::move( m_error );
    }

    EA_CPP14_CONSTEXPR error_type && error() &&
    {
        return eastl::move( m_error );
    }

    bool has_value() const
    {
        return m_has_value;
    }

    void set_has_value( bool v )
    {
        m_has_value = v;
    }

private:
    union
    {
        value_type m_value;
        error_type m_error;
    };

    bool m_has_value = false;
};

template< typename T, typename E >
class storage_t_impl
{
    template< typename, typename > friend class eastl::expected_lite::expected;

public:
    using value_type = T;
    using error_type = E;

    // no-op construction
    storage_t_impl() {}
    ~storage_t_impl() {}

    explicit storage_t_impl( bool has_value )
        : m_has_value( has_value )
    {}

    void construct_value()
    {
        new( &m_value ) value_type();
    }

    void construct_value( value_type const & e )
    {
        new( &m_value ) value_type( e );
    }

    void construct_value( value_type && e )
    {
        new( &m_value ) value_type(eastl::move( e ) );
    }

    template< class... Args >
    void emplace_value( Args&&... args )
    {
        new( &m_value ) value_type(eastl::forward<Args>(args)...);
    }

    template< class U, class... Args >
    void emplace_value( std::initializer_list<U> il, Args&&... args )
    {
        new( &m_value ) value_type( il, eastl::forward<Args>(args)... );
    }

    void destruct_value()
    {
        m_value.~value_type();
    }

    void construct_error( error_type const & e )
    {
        new( &m_error ) error_type( e );
    }

    void construct_error( error_type && e )
    {
        new( &m_error ) error_type(eastl::move( e ) );
    }

    template< class... Args >
    void emplace_error( Args&&... args )
    {
        new( &m_error ) error_type(eastl::forward<Args>(args)...);
    }

    template< class U, class... Args >
    void emplace_error( std::initializer_list<U> il, Args&&... args )
    {
        new( &m_error ) error_type( il, eastl::forward<Args>(args)... );
    }

    void destruct_error()
    {
        m_error.~error_type();
    }

    constexpr value_type const & value() const &
    {
        return m_value;
    }

    value_type & value() &
    {
        return m_value;
    }

    constexpr value_type const && value() const &&
    {
        return eastl::move( m_value );
    }

    EA_CPP14_CONSTEXPR value_type && value() &&
    {
        return eastl::move( m_value );
    }

    value_type const * value_ptr() const
    {
        return &m_value;
    }

    value_type * value_ptr()
    {
        return &m_value;
    }

    error_type const & error() const &
    {
        return m_error;
    }

    error_type & error() &
    {
        return m_error;
    }

    constexpr error_type const && error() const &&
    {
        return eastl::move( m_error );
    }

    EA_CPP14_CONSTEXPR error_type && error() &&
    {
        return eastl::move( m_error );
    }

    bool has_value() const
    {
        return m_has_value;
    }

    void set_has_value( bool v )
    {
        m_has_value = v;
    }

private:
    union
    {
        value_type m_value;
        error_type m_error;
    };

    bool m_has_value = false;
};

/// discriminated union to hold only 'error'.

template< typename E >
struct storage_t_impl<void, E>
{
    template< typename, typename > friend class eastl::expected_lite::expected;

public:
    using value_type = void;
    using error_type = E;

    // no-op construction
    storage_t_impl() {}
    ~storage_t_impl() {}

    explicit storage_t_impl( bool has_value )
        : m_has_value( has_value )
    {}

    void construct_error( error_type const & e )
    {
        new( &m_error ) error_type( e );
    }

    void construct_error( error_type && e )
    {
        new( &m_error ) error_type(eastl::move( e ) );
    }

    template< class... Args >
    void emplace_error( Args&&... args )
    {
        new( &m_error ) error_type(eastl::forward<Args>(args)...);
    }

    template< class U, class... Args >
    void emplace_error( std::initializer_list<U> il, Args&&... args )
    {
        new( &m_error ) error_type( il, eastl::forward<Args>(args)... );
    }

    void destruct_error()
    {
        m_error.~error_type();
    }

    error_type const & error() const &
    {
        return m_error;
    }

    error_type & error() &
    {
        return m_error;
    }

    constexpr error_type const && error() const &&
    {
        return eastl::move( m_error );
    }

    EA_CPP14_CONSTEXPR error_type && error() &&
    {
        return eastl::move( m_error );
    }

    bool has_value() const
    {
        return m_has_value;
    }

    void set_has_value( bool v )
    {
        m_has_value = v;
    }

private:
    union
    {
        char m_dummy;
        error_type m_error;
    };

    bool m_has_value = false;
};

template< typename T, typename E, bool isConstructable, bool isMoveable >
class storage_t
{
public:
};

template< typename T, typename E >
class storage_t<T, E, false, false> : public storage_t_noncopy_nonmove_impl<T, E>
{
public:
    storage_t() = default;
    ~storage_t() = default;

    explicit storage_t( bool has_value )
        : storage_t_noncopy_nonmove_impl<T, E>( has_value )
    {}

    storage_t( storage_t const & other ) = delete;
    storage_t( storage_t &&      other ) = delete;

};

template< typename T, typename E >
class storage_t<T, E, true, true> : public storage_t_impl<T, E>
{
public:
    storage_t() = default;
    ~storage_t() = default;

    explicit storage_t( bool has_value )
        : storage_t_impl<T, E>( has_value )
    {}

    storage_t( storage_t const & other )
        : storage_t_impl<T, E>( other.has_value() )
    {
        if ( this->has_value() ) this->construct_value( other.value() );
        else                     this->construct_error( other.error() );
    }

    storage_t(storage_t && other )
        : storage_t_impl<T, E>( other.has_value() )
    {
        if ( this->has_value() ) this->construct_value( eastl::move( other.value() ) );
        else                     this->construct_error( eastl::move( other.error() ) );
    }
};

template< typename E >
class storage_t<void, E, true, true> : public storage_t_impl<void, E>
{
public:
    storage_t() = default;
    ~storage_t() = default;

    explicit storage_t( bool has_value )
        : storage_t_impl<void, E>( has_value )
    {}

    storage_t( storage_t const & other )
        : storage_t_impl<void, E>( other.has_value() )
    {
        if ( this->has_value() ) ;
        else                     this->construct_error( other.error() );
    }

    storage_t(storage_t && other )
        : storage_t_impl<void, E>( other.has_value() )
    {
        if ( this->has_value() ) ;
        else                     this->construct_error(eastl::move( other.error() ) );
    }
};

template< typename T, typename E >
class storage_t<T, E, true, false> : public storage_t_impl<T, E>
{
public:
    storage_t() = default;
    ~storage_t() = default;

    explicit storage_t( bool has_value )
        : storage_t_impl<T, E>( has_value )
    {}

    storage_t( storage_t const & other )
        : storage_t_impl<T, E>(other.has_value())
    {
        if ( this->has_value() ) this->construct_value( other.value() );
        else                     this->construct_error( other.error() );
    }

    storage_t( storage_t && other ) = delete;
};

template< typename E >
class storage_t<void, E, true, false> : public storage_t_impl<void, E>
{
public:
    storage_t() = default;
    ~storage_t() = default;

    explicit storage_t( bool has_value )
        : storage_t_impl<void, E>( has_value )
    {}

    storage_t( storage_t const & other )
        : storage_t_impl<void, E>(other.has_value())
    {
        if ( this->has_value() ) ;
        else                     this->construct_error( other.error() );
    }

    storage_t( storage_t && other ) = delete;
};

template< typename T, typename E >
class storage_t<T, E, false, true> : public storage_t_impl<T, E>
{
public:
    storage_t() = default;
    ~storage_t() = default;

    explicit storage_t( bool has_value )
        : storage_t_impl<T, E>( has_value )
    {}

    storage_t( storage_t const & other ) = delete;

    storage_t( storage_t && other )
        : storage_t_impl<T, E>( other.has_value() )
    {
        if ( this->has_value() ) this->construct_value( eastl::move( other.value() ) );
        else                     this->construct_error( eastl::move( other.error() ) );
    }
};

template< typename E >
class storage_t<void, E, false, true> : public storage_t_impl<void, E>
{
public:
    storage_t() = default;
    ~storage_t() = default;

    explicit storage_t( bool has_value )
        : storage_t_impl<void, E>( has_value )
    {}

    storage_t( storage_t const & other ) = delete;

    storage_t( storage_t && other )
        : storage_t_impl<void, E>( other.has_value() )
    {
        if ( this->has_value() ) ;
        else                     this->construct_error(eastl::move( other.error() ) );
    }
};

#if nsel_P2505R >= 3
// C++11 invoke implementation
template< typename >
struct is_reference_wrapper : eastl::false_type {};
template< typename T >
struct is_reference_wrapper<eastl::reference_wrapper< T > > : eastl::true_type {};

template< typename FnT, typename ClassT, typename ObjectT, typename... Args
    nsel_REQUIRES_T(
        eastl::is_function<FnT>::value
        && (eastl::is_same< ClassT, typename eastl::remove_cvref< ObjectT >::type >::value
        || eastl::is_base_of< ClassT, typename eastl::remove_cvref< ObjectT >::type >::value )
    )
>
EA_CONSTEXPR auto invoke_member_function_impl( FnT ClassT::* memfnptr, ObjectT && obj, Args && ... args )
        noexcept( noexcept( (eastl::forward< ObjectT >( obj ).*memfnptr)(eastl::forward< Args >( args )... ) ) )
        -> decltype( (eastl::forward< ObjectT >( obj ).*memfnptr)(eastl::forward< Args >( args )...) )
{
      return (eastl::forward< ObjectT >( obj ).*memfnptr)(eastl::forward< Args >( args )... );
}

template< typename FnT, typename ClassT, typename ObjectT, typename... Args
    nsel_REQUIRES_T(
        eastl::is_function<FnT>::value
        && is_reference_wrapper< typename eastl::remove_cvref< ObjectT >::type >::value
    )
>
EA_CONSTEXPR auto invoke_member_function_impl( FnT ClassT::* memfnptr, ObjectT && obj, Args && ... args )
        noexcept( noexcept( (obj.get().*memfnptr)(eastl::forward< Args >( args ) ... ) ) )
        -> decltype( (obj.get().*memfnptr)(eastl::forward< Args >( args ) ... ) )
{
    return (obj.get().*memfnptr)(eastl::forward< Args >( args ) ... );
}

template< typename FnT, typename ClassT, typename ObjectT, typename... Args
    nsel_REQUIRES_T(
        eastl::is_function<FnT>::value
        && !eastl::is_same< ClassT, typename eastl::remove_cvref< ObjectT >::type >::value
        && !eastl::is_base_of< ClassT, typename eastl::remove_cvref< ObjectT >::type >::value
        && !is_reference_wrapper< typename eastl::remove_cvref< ObjectT >::type >::value
    )
>
EA_CONSTEXPR auto invoke_member_function_impl( FnT ClassT::* memfnptr, ObjectT && obj, Args && ... args )
        noexcept( noexcept( ((*eastl::forward< ObjectT >( obj )).*memfnptr)(eastl::forward< Args >( args ) ... ) ) )
        -> decltype( ((*eastl::forward< ObjectT >( obj )).*memfnptr)(eastl::forward< Args >( args ) ... ) )
{
    return ((*eastl::forward<ObjectT>(obj)).*memfnptr)(eastl::forward< Args >( args ) ... );
}

template< typename MemberT, typename ClassT, typename ObjectT
    nsel_REQUIRES_T(
        eastl::is_same< ClassT, typename eastl::remove_cvref< ObjectT >::type >::value
        || eastl::is_base_of< ClassT, typename eastl::remove_cvref< ObjectT >::type >::value
    )
>
EA_CONSTEXPR auto invoke_member_object_impl( MemberT ClassT::* memobjptr, ObjectT && obj )
        noexcept( noexcept(eastl::forward< ObjectT >( obj ).*memobjptr ) )
        -> decltype(eastl::forward< ObjectT >( obj ).*memobjptr )
{
    return eastl::forward< ObjectT >( obj ).*memobjptr;
}

template< typename MemberT, typename ClassT, typename ObjectT
    nsel_REQUIRES_T(
        is_reference_wrapper< typename eastl::remove_cvref< ObjectT >::type >::value
    )
>
EA_CONSTEXPR auto invoke_member_object_impl( MemberT ClassT::* memobjptr, ObjectT && obj )
        noexcept( noexcept( obj.get().*memobjptr ) )
        -> decltype( obj.get().*memobjptr )
{
    return obj.get().*memobjptr;
}

template< typename MemberT, typename ClassT, typename ObjectT
    nsel_REQUIRES_T(
        !eastl::is_same< ClassT, typename eastl::remove_cvref< ObjectT >::type >::value
        && !eastl::is_base_of< ClassT, typename eastl::remove_cvref< ObjectT >::type >::value
        && !is_reference_wrapper< typename eastl::remove_cvref< ObjectT >::type >::value
    )
>
EA_CONSTEXPR auto invoke_member_object_impl( MemberT ClassT::* memobjptr, ObjectT && obj )
        noexcept( noexcept( (*eastl::forward< ObjectT >( obj )).*memobjptr ) )
        -> decltype( (*eastl::forward< ObjectT >( obj )).*memobjptr )
{
    return (*eastl::forward< ObjectT >( obj )).*memobjptr;
}

template< typename F, typename... Args
    nsel_REQUIRES_T(
        eastl::is_member_function_pointer< typename eastl::remove_cvref< F >::type >::value
    )
>
EA_CONSTEXPR auto invoke( F && f, Args && ... args )
        noexcept( noexcept( invoke_member_function_impl(eastl::forward< F >( f ), eastl::forward< Args >( args ) ... ) ) )
        -> decltype( invoke_member_function_impl(eastl::forward< F >( f ), eastl::forward< Args >( args ) ... ) )
{
    return invoke_member_function_impl(eastl::forward< F >( f ), eastl::forward< Args >( args ) ... );
}

template< typename F, typename... Args
    nsel_REQUIRES_T(
        eastl::is_member_object_pointer< typename eastl::remove_cvref< F >::type >::value
    )
>
EA_CONSTEXPR auto invoke( F && f, Args && ... args )
        noexcept( noexcept( invoke_member_object_impl(eastl::forward< F >( f ), eastl::forward< Args >( args ) ... ) ) )
        -> decltype( invoke_member_object_impl(eastl::forward< F >( f ), eastl::forward< Args >( args ) ... ) )
{
    return invoke_member_object_impl(eastl::forward< F >( f ), eastl::forward< Args >( args ) ... );
}

template< typename F, typename... Args
    nsel_REQUIRES_T(
        !eastl::is_member_function_pointer< typename eastl::remove_cvref< F >::type >::value
        && !eastl::is_member_object_pointer< typename eastl::remove_cvref< F >::type >::value
    )
>
EA_CONSTEXPR auto invoke( F && f, Args && ... args )
        noexcept( noexcept(eastl::forward< F >( f )(eastl::forward< Args >( args ) ... ) ) )
        -> decltype(eastl::forward< F >( f )(eastl::forward< Args >( args ) ... ) )
{
    return eastl::forward< F >( f )(eastl::forward< Args >( args ) ... );
}

template< typename F, typename ... Args >
using invoke_result_nocvref_t = typename eastl::remove_cvref< decltype( invoke(eastl::declval< F >(), eastl::declval< Args >()... ) ) >::type;

#if nsel_P2505R >= 5
template< typename F, typename ... Args >
using transform_invoke_result_t = typename eastl::remove_cv< decltype( invoke(eastl::declval< F >(), eastl::declval< Args >()... ) ) >::type;
#else
template< typename F, typename ... Args >
using transform_invoke_result_t = invoke_result_nocvref_t
#endif // nsel_P2505R >= 5

template< typename T >
struct valid_expected_value_type : eastl::integral_constant< bool, eastl::is_destructible< T >::value && !eastl::is_reference< T >::value && !eastl::is_array< T >::value > {};

#endif // nsel_P2505R >= 3
} // namespace detail

/// x.x.5 Unexpected object type; unexpected_type; C++17 and later can also use aliased type unexpected.

#if nsel_P0323R <= 2
template< typename E = std::exception_ptr >
class unexpected_type
#else
template< typename E >
class unexpected_type
#endif // nsel_P0323R
{
public:
    using error_type = E;

    // x.x.5.2.1 Constructors

//  unexpected_type() = delete;

    constexpr unexpected_type( unexpected_type const & ) = default;
    constexpr unexpected_type( unexpected_type && ) = default;

    template< typename... Args
        nsel_REQUIRES_T(
            eastl::is_constructible<E, Args&&...>::value
        )
    >
    constexpr explicit unexpected_type( eastl_lite_in_place_t(E), Args &&... args )
    : m_error(eastl::forward<Args>( args )...)
    {}

    template< typename U, typename... Args
        nsel_REQUIRES_T(
            eastl::is_constructible<E, std::initializer_list<U>, Args&&...>::value
        )
    >
    constexpr explicit unexpected_type( eastl_lite_in_place_t(E), std::initializer_list<U> il, Args &&... args )
    : m_error( il, eastl::forward<Args>( args )...)
    {}

    template< typename E2
        nsel_REQUIRES_T(
            eastl::is_constructible<E,E2>::value
            && !eastl::is_same< typename eastl::remove_cvref<E2>::type, eastl_lite_in_place_t(E2) >::value
            && !eastl::is_same< typename eastl::remove_cvref<E2>::type, unexpected_type >::value
        )
    >
    constexpr explicit unexpected_type( E2 && error )
    : m_error(eastl::forward<E2>( error ) )
    {}

    template< typename E2
        nsel_REQUIRES_T(
            eastl::is_constructible<    E, E2>::value
            && !eastl::is_constructible<E, unexpected_type<E2>       &   >::value
            && !eastl::is_constructible<E, unexpected_type<E2>           >::value
            && !eastl::is_constructible<E, unexpected_type<E2> const &   >::value
            && !eastl::is_constructible<E, unexpected_type<E2> const     >::value
            && !eastl::is_convertible<     unexpected_type<E2>       &, E>::value
            && !eastl::is_convertible<     unexpected_type<E2>        , E>::value
            && !eastl::is_convertible<     unexpected_type<E2> const &, E>::value
            && !eastl::is_convertible<     unexpected_type<E2> const  , E>::value
            && !eastl::is_convertible< E2 const &, E>::value /*=> explicit */
        )
    >
    constexpr explicit unexpected_type( unexpected_type<E2> const & error )
    : m_error( E{ error.error() } )
    {}

    template< typename E2
        nsel_REQUIRES_T(
            eastl::is_constructible<    E, E2>::value
            && !eastl::is_constructible<E, unexpected_type<E2>       &   >::value
            && !eastl::is_constructible<E, unexpected_type<E2>           >::value
            && !eastl::is_constructible<E, unexpected_type<E2> const &   >::value
            && !eastl::is_constructible<E, unexpected_type<E2> const     >::value
            && !eastl::is_convertible<     unexpected_type<E2>       &, E>::value
            && !eastl::is_convertible<     unexpected_type<E2>        , E>::value
            && !eastl::is_convertible<     unexpected_type<E2> const &, E>::value
            && !eastl::is_convertible<     unexpected_type<E2> const  , E>::value
            &&  eastl::is_convertible< E2 const &, E>::value /*=> explicit */
        )
    >
    constexpr /*non-explicit*/ unexpected_type( unexpected_type<E2> const & error )
    : m_error( error.error() )
    {}

    template< typename E2
        nsel_REQUIRES_T(
            eastl::is_constructible<    E, E2>::value
            && !eastl::is_constructible<E, unexpected_type<E2>       &   >::value
            && !eastl::is_constructible<E, unexpected_type<E2>           >::value
            && !eastl::is_constructible<E, unexpected_type<E2> const &   >::value
            && !eastl::is_constructible<E, unexpected_type<E2> const     >::value
            && !eastl::is_convertible<     unexpected_type<E2>       &, E>::value
            && !eastl::is_convertible<     unexpected_type<E2>        , E>::value
            && !eastl::is_convertible<     unexpected_type<E2> const &, E>::value
            && !eastl::is_convertible<     unexpected_type<E2> const  , E>::value
            && !eastl::is_convertible< E2 const &, E>::value /*=> explicit */
        )
    >
    constexpr explicit unexpected_type( unexpected_type<E2> && error )
    : m_error( E{ eastl::move( error.error() ) } )
    {}

    template< typename E2
        nsel_REQUIRES_T(
            eastl::is_constructible<    E, E2>::value
            && !eastl::is_constructible<E, unexpected_type<E2>       &   >::value
            && !eastl::is_constructible<E, unexpected_type<E2>           >::value
            && !eastl::is_constructible<E, unexpected_type<E2> const &   >::value
            && !eastl::is_constructible<E, unexpected_type<E2> const     >::value
            && !eastl::is_convertible<     unexpected_type<E2>       &, E>::value
            && !eastl::is_convertible<     unexpected_type<E2>        , E>::value
            && !eastl::is_convertible<     unexpected_type<E2> const &, E>::value
            && !eastl::is_convertible<     unexpected_type<E2> const  , E>::value
            &&  eastl::is_convertible< E2 const &, E>::value /*=> non-explicit */
        )
    >
    constexpr /*non-explicit*/ unexpected_type( unexpected_type<E2> && error )
    : m_error( eastl::move( error.error() ) )
    {}

    // x.x.5.2.2 Assignment

    EA_CPP14_CONSTEXPR unexpected_type& operator=( unexpected_type const & ) = default;
    EA_CPP14_CONSTEXPR unexpected_type& operator=( unexpected_type && ) = default;

    template< typename E2 = E >
    EA_CPP14_CONSTEXPR unexpected_type & operator=( unexpected_type<E2> const & other )
    {
        unexpected_type{ other.error() }.swap( *this );
        return *this;
    }

    template< typename E2 = E >
    EA_CPP14_CONSTEXPR unexpected_type & operator=( unexpected_type<E2> && other )
    {
        unexpected_type{ eastl::move( other.error() ) }.swap( *this );
        return *this;
    }

    // x.x.5.2.3 Observers

    EA_CPP14_CONSTEXPR E & error() & noexcept
    {
        return m_error;
    }

    constexpr E const & error() const & noexcept
    {
        return m_error;
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490

    EA_CPP14_CONSTEXPR E && error() && noexcept
    {
        return eastl::move( m_error );
    }

    constexpr E const && error() const && noexcept
    {
        return eastl::move( m_error );
    }

#endif

    // x.x.5.2.3 Observers - deprecated

    nsel_deprecated("replace value() with error()")

    EA_CPP14_CONSTEXPR E & value() & noexcept
    {
        return m_error;
    }

    nsel_deprecated("replace value() with error()")

    constexpr E const & value() const & noexcept
    {
        return m_error;
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490

    nsel_deprecated("replace value() with error()")

    EA_CPP14_CONSTEXPR E && value() && noexcept
    {
        return eastl::move( m_error );
    }

    nsel_deprecated("replace value() with error()")

    constexpr E const && value() const && noexcept
    {
        return eastl::move( m_error );
    }

#endif

    // x.x.5.2.4 Swap

    template< typename U=E >
    nsel_REQUIRES_R( void,
        eastl::is_swappable<U>::value
    )
    swap( unexpected_type & other ) noexcept (eastl::::is_nothrow_swappable<U>::value)
    {
        using eastl::swap;
        swap( m_error, other.m_error );
    }

    // TODO: ??? unexpected_type: in-class friend operator==, !=

private:
    error_type m_error;
};

#if nsel_CPP17_OR_GREATER

/// template deduction guide:

template< typename E >
unexpected_type( E ) -> unexpected_type< E >;

#endif

/// x.x.4, Unexpected equality operators

template< typename E1, typename E2 >
constexpr bool operator==( unexpected_type<E1> const & x, unexpected_type<E2> const & y )
{
    return x.error() == y.error();
}

template< typename E1, typename E2 >
constexpr bool operator!=( unexpected_type<E1> const & x, unexpected_type<E2> const & y )
{
    return ! ( x == y );
}

#if nsel_P0323R <= 2

template< typename E >
constexpr bool operator<( unexpected_type<E> const & x, unexpected_type<E> const & y )
{
    return x.error() < y.error();
}

template< typename E >
constexpr bool operator>( unexpected_type<E> const & x, unexpected_type<E> const & y )
{
    return ( y < x );
}

template< typename E >
constexpr bool operator<=( unexpected_type<E> const & x, unexpected_type<E> const & y )
{
    return ! ( y < x  );
}

template< typename E >
constexpr bool operator>=( unexpected_type<E> const & x, unexpected_type<E> const & y )
{
    return ! ( x < y );
}

#endif // nsel_P0323R

/// x.x.5 Specialized algorithms

template< typename E
    nsel_REQUIRES_T(
        eastl::is_swappable<E>::value
    )
>
void swap( unexpected_type<E> & x, unexpected_type<E> & y) noexcept ( noexcept ( x.swap(y) ) )
{
    x.swap( y );
}

#if nsel_P0323R <= 2

// unexpected: relational operators for std::exception_ptr:

inline constexpr bool operator<( unexpected_type<std::exception_ptr> const & /*x*/, unexpected_type<std::exception_ptr> const & /*y*/ )
{
    return false;
}

inline constexpr bool operator>( unexpected_type<std::exception_ptr> const & /*x*/, unexpected_type<std::exception_ptr> const & /*y*/ )
{
    return false;
}

inline constexpr bool operator<=( unexpected_type<std::exception_ptr> const & x, unexpected_type<std::exception_ptr> const & y )
{
    return ( x == y );
}

inline constexpr bool operator>=( unexpected_type<std::exception_ptr> const & x, unexpected_type<std::exception_ptr> const & y )
{
    return ( x == y );
}

#endif // nsel_P0323R

// unexpected: traits

#if nsel_P0323R <= 3

template< typename E>
struct is_unexpected : eastl::false_type {};

template< typename E>
struct is_unexpected< unexpected_type<E> > : eastl::true_type {};

#endif // nsel_P0323R

// unexpected: factory

// keep make_unexpected() removed in p0323r2 for pre-C++17:

template< typename E>
EA_CPP14_CONSTEXPR auto
make_unexpected( E && value ) -> unexpected_type< typename eastl::decay<E>::type >
{
    return unexpected_type< typename eastl::decay<E>::type >(eastl::forward<E>(value) );
}
/*BUGBUGBUG
template <typename E, typename... Args, typename = eastl::enable_if<eastl::is_constructible<E, Args...>::value>>
EA_CPP14_CONSTEXPR auto
make_unexpected( nonstd_lite_in_place_t(E), Args &&... args ) -> unexpected_type< typename eastl::decay<E>::type >
{
    return eastl::move( unexpected_type< typename eastl::decay<E>::type >( nonstd_lite_in_place(E), eastl::forward<Args>(args)...) );
}
*/

#if nsel_P0323R <= 3

/*EA_CPP14_CONSTEXPR*/ auto inline
make_unexpected_from_current_exception() -> unexpected_type< std::exception_ptr >
{
    return unexpected_type< std::exception_ptr >( std::current_exception() );
}

#endif // nsel_P0323R

/// x.x.6, x.x.7 expected access error

template< typename E >
class bad_expected_access;

/// x.x.7 bad_expected_access<void>: expected access error

template <>
class bad_expected_access< void > : public std::exception
{
public:
    explicit bad_expected_access()
    : std::exception()
    {}
};

/// x.x.6 bad_expected_access: expected access error

#if !nsel_CONFIG_NO_EXCEPTIONS

template< typename E >
class bad_expected_access : public bad_expected_access< void >
{
public:
    using error_type = E;

    explicit bad_expected_access( error_type error )
    : m_error( error )
    {}

    virtual char const * what() const noexcept override
    {
        return "bad_expected_access";
    }

    EA_CPP14_CONSTEXPR error_type & error() &
    {
        return m_error;
    }

    constexpr error_type const & error() const &
    {
        return m_error;
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490

    EA_CPP14_CONSTEXPR error_type && error() &&
    {
        return eastl::move( m_error );
    }

    constexpr error_type const && error() const &&
    {
        return eastl::move( m_error );
    }

#endif

private:
    error_type m_error;
};

#endif // nsel_CONFIG_NO_EXCEPTIONS

/// x.x.8 unexpect tag, in_place_unexpected tag: construct an error

struct unexpect_t{};
using in_place_unexpected_t = unexpect_t;

nsel_inline17 constexpr unexpect_t unexpect{};
nsel_inline17 constexpr unexpect_t in_place_unexpected{};

/// class error_traits


template< typename Error >
struct error_traits
{
    static void rethrow( Error const & e )
    {
        throw bad_expected_access<Error>{ e };
    }
};

template<>
struct error_traits< std::exception_ptr >
{
    static void rethrow( std::exception_ptr const & e )
    {
        std::rethrow_exception( e );
    }
};

template<>
struct error_traits< std::error_code >
{
    static void rethrow( std::error_code const & e )
    {
        throw std::system_error( e );
    }
};

#if nsel_P2505R >= 3
namespace detail {

// from https://en.cppreference.com/w/cpp/utility/expected/unexpected:
// "the type of the unexpected value. The type must not be an array type, a non-object type, a specialization of eastl::unexpected, or a cv-qualified type."
template< typename T >
struct valid_unexpected_type : eastl::integral_constant< bool,
    eastl::is_same< T, typename eastl::remove_cvref< T >::type >::value
    && eastl::is_object< T >::value
    && !eastl::is_array< T >::value
> {};

template< typename T >
struct valid_unexpected_type< unexpected_type< T > > : eastl::false_type {};

} // namespace detail
#endif // nsel_P2505R >= 3

} // namespace expected_lite

// provide eastl::unexpected_type:

using expected_lite::unexpected_type;

namespace expected_lite {

/// class expected

#if nsel_P0323R <= 2
template< typename T, typename E = std::exception_ptr >
class expected
#else
template< typename T, typename E >
class expected
#endif // nsel_P0323R
{
private:
    template< typename, typename > friend class expected;

public:
    using value_type = T;
    using error_type = E;
    using unexpected_type = eastl::unexpected_type<E>;

    template< typename U >
    struct rebind
    {
        using type = expected<U, error_type>;
    };

    // x.x.4.1 constructors

    nsel_REQUIRES_0(
        eastl::is_default_constructible<T>::value
    )
    EA_CPP14_CONSTEXPR expected()
    : contained( true )
    {
        contained.construct_value();
    }

    EA_CPP14_CONSTEXPR expected( expected const & ) = default;
    EA_CPP14_CONSTEXPR expected( expected &&      ) = default;

    template< typename U, typename G
        nsel_REQUIRES_T(
            eastl::is_constructible<    T, U const &>::value
            &&  eastl::is_constructible<E, G const &>::value
            && !eastl::is_constructible<T, expected<U, G>       &    >::value
            && !eastl::is_constructible<T, expected<U, G>       &&   >::value
            && !eastl::is_constructible<T, expected<U, G> const &    >::value
            && !eastl::is_constructible<T, expected<U, G> const &&   >::value
            && !eastl::is_convertible<     expected<U, G>       & , T>::value
            && !eastl::is_convertible<     expected<U, G>       &&, T>::value
            && !eastl::is_convertible<     expected<U, G> const & , T>::value
            && !eastl::is_convertible<     expected<U, G> const &&, T>::value
            && (!eastl::is_convertible<U const &, T>::value || !eastl::is_convertible<G const &, E>::value ) /*=> explicit */
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( expected<U, G> const & other )
    : contained( other.has_value() )
    {
        if ( has_value() ) contained.construct_value( T{ other.contained.value() } );
        else               contained.construct_error( E{ other.contained.error() } );
    }

    template< typename U, typename G
        nsel_REQUIRES_T(
            eastl::is_constructible<    T, U const &>::value
            &&  eastl::is_constructible<E, G const &>::value
            && !eastl::is_constructible<T, expected<U, G>       &    >::value
            && !eastl::is_constructible<T, expected<U, G>       &&   >::value
            && !eastl::is_constructible<T, expected<U, G> const &    >::value
            && !eastl::is_constructible<T, expected<U, G> const &&   >::value
            && !eastl::is_convertible<     expected<U, G>       & , T>::value
            && !eastl::is_convertible<     expected<U, G>       &&, T>::value
            && !eastl::is_convertible<     expected<U, G> const  &, T>::value
            && !eastl::is_convertible<     expected<U, G> const &&, T>::value
            && !(!eastl::is_convertible<U const &, T>::value || !eastl::is_convertible<G const &, E>::value ) /*=> non-explicit */
        )
    >
    EA_CPP14_CONSTEXPR /*non-explicit*/ expected( expected<U, G> const & other )
    : contained( other.has_value() )
    {
        if ( has_value() ) contained.construct_value( other.contained.value() );
        else               contained.construct_error( other.contained.error() );
    }

    template< typename U, typename G
        nsel_REQUIRES_T(
            eastl::is_constructible<    T, U>::value
            &&  eastl::is_constructible<E, G>::value
            && !eastl::is_constructible<T, expected<U, G>       &    >::value
            && !eastl::is_constructible<T, expected<U, G>       &&   >::value
            && !eastl::is_constructible<T, expected<U, G> const &    >::value
            && !eastl::is_constructible<T, expected<U, G> const &&   >::value
            && !eastl::is_convertible<     expected<U, G>       & , T>::value
            && !eastl::is_convertible<     expected<U, G>       &&, T>::value
            && !eastl::is_convertible<     expected<U, G> const & , T>::value
            && !eastl::is_convertible<     expected<U, G> const &&, T>::value
            && (!eastl::is_convertible<U, T>::value || !eastl::is_convertible<G, E>::value ) /*=> explicit */
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( expected<U, G> && other )
    : contained( other.has_value() )
    {
        if ( has_value() ) contained.construct_value( T{ eastl::move( other.contained.value() ) } );
        else               contained.construct_error( E{ eastl::move( other.contained.error() ) } );
    }

    template< typename U, typename G
        nsel_REQUIRES_T(
            eastl::is_constructible<    T, U>::value
            &&  eastl::is_constructible<E, G>::value
            && !eastl::is_constructible<T, expected<U, G>      &     >::value
            && !eastl::is_constructible<T, expected<U, G>      &&    >::value
            && !eastl::is_constructible<T, expected<U, G> const &    >::value
            && !eastl::is_constructible<T, expected<U, G> const &&   >::value
            && !eastl::is_convertible<     expected<U, G>       & , T>::value
            && !eastl::is_convertible<     expected<U, G>       &&, T>::value
            && !eastl::is_convertible<     expected<U, G> const & , T>::value
            && !eastl::is_convertible<     expected<U, G> const &&, T>::value
            && !(!eastl::is_convertible<U, T>::value || !eastl::is_convertible<G, E>::value ) /*=> non-explicit */
        )
    >
    EA_CPP14_CONSTEXPR /*non-explicit*/ expected( expected<U, G> && other )
    : contained( other.has_value() )
    {
        if ( has_value() ) contained.construct_value( eastl::move( other.contained.value() ) );
        else               contained.construct_error( eastl::move( other.contained.error() ) );
    }

    template< typename U = T
        nsel_REQUIRES_T(
            eastl::is_copy_constructible<U>::value
        )
    >
    EA_CPP14_CONSTEXPR expected( value_type const & value )
    : contained( true )
    {
        contained.construct_value( value );
    }

    template< typename U = T
        nsel_REQUIRES_T(
            eastl::is_constructible<T,U&&>::value
            && !eastl::is_same<typename eastl::remove_cvref<U>::type, eastl_lite_in_place_t(U)>::value
            && !eastl::is_same<        expected<T,E>     , typename eastl::remove_cvref<U>::type>::value
            && !eastl::is_same<eastl::unexpected_type<E>, typename eastl::remove_cvref<U>::type>::value
            && !eastl::is_convertible<U&&,T>::value /*=> explicit */
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( U && value ) noexcept
    (
        eastl::is_nothrow_move_constructible<U>::value &&
        eastl::is_nothrow_move_constructible<E>::value
    )
    : contained( true )
    {
        contained.construct_value( T{ eastl::forward<U>( value ) } );
    }

    template< typename U = T
        nsel_REQUIRES_T(
            eastl::is_constructible<T,U&&>::value
            && !eastl::is_same<typename eastl::remove_cvref<U>::type, eastl_lite_in_place_t(U)>::value
            && !eastl::is_same<        expected<T,E>     , typename eastl::remove_cvref<U>::type>::value
            && !eastl::is_same<eastl::unexpected_type<E>, typename eastl::remove_cvref<U>::type>::value
            && eastl::is_convertible<U&&,T>::value /*=> non-explicit */
        )
    >
    EA_CPP14_CONSTEXPR /*non-explicit*/ expected( U && value ) noexcept
    (
        eastl::is_nothrow_move_constructible<U>::value &&
        eastl::is_nothrow_move_constructible<E>::value
    )
    : contained( true )
    {
        contained.construct_value(eastl::forward<U>( value ) );
    }

    // construct error:

    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_constructible<E, G const &   >::value
            && !eastl::is_convertible< G const &, E>::value /*=> explicit */
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( eastl::unexpected_type<G> const & error )
    : contained( false )
    {
        contained.construct_error( E{ error.error() } );
    }

    template< typename G = E
            nsel_REQUIRES_T(
                eastl::is_constructible<E, G const &   >::value
            && eastl::is_convertible<  G const &, E>::value /*=> non-explicit */
        )
    >
    EA_CPP14_CONSTEXPR /*non-explicit*/ expected( eastl::unexpected_type<G> const & error )
    : contained( false )
    {
        contained.construct_error( error.error() );
    }

    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_constructible<E, G&&   >::value
            && !eastl::is_convertible< G&&, E>::value /*=> explicit */
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( eastl::unexpected_type<G> && error )
    : contained( false )
    {
        contained.construct_error( E{ eastl::move( error.error() ) } );
    }

    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_constructible<E, G&&   >::value
            && eastl::is_convertible<  G&&, E>::value /*=> non-explicit */
        )
    >
    EA_CPP14_CONSTEXPR /*non-explicit*/ expected( eastl::unexpected_type<G> && error )
    : contained( false )
    {
        contained.construct_error( eastl::move( error.error() ) );
    }

    // in-place construction, value

    template< typename... Args
        nsel_REQUIRES_T(
            eastl::is_constructible<T, Args&&...>::value
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( eastl_lite_in_place_t(T), Args&&... args )
    : contained( true )
    {
        contained.emplace_value(eastl::forward<Args>( args )... );
    }

    template< typename U, typename... Args
        nsel_REQUIRES_T(
            eastl::is_constructible<T, std::initializer_list<U>, Args&&...>::value
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( eastl_lite_in_place_t(T), std::initializer_list<U> il, Args&&... args )
    : contained( true )
    {
        contained.emplace_value( il, eastl::forward<Args>( args )... );
    }

    // in-place construction, error

    template< typename... Args
        nsel_REQUIRES_T(
            eastl::is_constructible<E, Args&&...>::value
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( unexpect_t, Args&&... args )
    : contained( false )
    {
        contained.emplace_error(eastl::forward<Args>( args )... );
    }

    template< typename U, typename... Args
        nsel_REQUIRES_T(
            eastl::is_constructible<E, std::initializer_list<U>, Args&&...>::value
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( unexpect_t, std::initializer_list<U> il, Args&&... args )
    : contained( false )
    {
        contained.emplace_error( il, eastl::forward<Args>( args )... );
    }

    // x.x.4.2 destructor

    // TODO: ~expected: triviality
    // Effects: If T is not cv void and is_trivially_destructible_v<T> is false and bool(*this), calls val.~T(). If is_trivially_destructible_v<E> is false and !bool(*this), calls unexpect.~unexpected<E>().
    // Remarks: If either T is cv void or is_trivially_destructible_v<T> is true, and is_trivially_destructible_v<E> is true, then this destructor shall be a trivial destructor.

    ~expected()
    {
        if ( has_value() ) contained.destruct_value();
        else               contained.destruct_error();
    }

    // x.x.4.3 assignment

    expected & operator=( expected const & other )
    {
        expected( other ).swap( *this );
        return *this;
    }

    expected & operator=( expected && other ) noexcept
    (
        eastl::is_nothrow_move_constructible<   T>::value
        && eastl::is_nothrow_move_assignable<   T>::value
        && eastl::is_nothrow_move_constructible<E>::value     // added for missing
        && eastl::is_nothrow_move_assignable<   E>::value )   //   nothrow above
    {
        expected(eastl::move( other ) ).swap( *this );
        return *this;
    }

    template< typename U
        nsel_REQUIRES_T(
            !eastl::is_same<expected<T,E>, typename eastl::remove_cvref<U>::type>::value
            && eastl::conjunction<eastl::is_scalar<T>, eastl::is_same<T, eastl::decay<U>> >::value
            && eastl::is_constructible<T ,U>::value
            && eastl::is_assignable<   T&,U>::value
            && eastl::is_nothrow_move_constructible<E>::value )
    >
    expected & operator=( U && value )
    {
        expected(eastl::forward<U>( value ) ).swap( *this );
        return *this;
    }

    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_constructible<E, G const&>::value &&
            eastl::is_copy_constructible<G>::value    // TODO: eastl::is_nothrow_copy_constructible<G>
            && eastl::is_copy_assignable<G>::value
        )
    >
    expected & operator=( eastl::unexpected_type<G> const & error )
    {
        expected( unexpect, error.error() ).swap( *this );
        return *this;
    }

    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_constructible<E, G&&>::value &&
            eastl::is_move_constructible<G>::value    // TODO: eastl::is_nothrow_move_constructible<G>
            && eastl::is_move_assignable<G>::value
        )
    >
    expected & operator=( eastl::unexpected_type<G> && error )
    {
        expected( unexpect, eastl::move( error.error() ) ).swap( *this );
        return *this;
    }

    template< typename... Args
        nsel_REQUIRES_T(
            eastl::is_nothrow_constructible<T, Args&&...>::value
        )
    >
    value_type & emplace( Args &&... args )
    {
        expected( eastl_lite_in_place(T), eastl::forward<Args>(args)... ).swap( *this );
        return value();
    }

    template< typename U, typename... Args
        nsel_REQUIRES_T(
            eastl::is_nothrow_constructible<T, std::initializer_list<U>&, Args&&...>::value
        )
    >
    value_type & emplace( std::initializer_list<U> il, Args &&... args )
    {
        expected( eastl_lite_in_place(T), il, eastl::forward<Args>(args)... ).swap( *this );
        return value();
    }

    // x.x.4.4 swap

    template< typename U=T, typename G=E >
    nsel_REQUIRES_R( void,
        eastl::is_swappable<   U>::value
        && eastl::is_swappable<G>::value
        && (eastl::is_move_constructible<U>::value || eastl::is_move_constructible<G>::value )
    )
    swap( expected & other ) noexcept
    (
        eastl::is_nothrow_move_constructible<T>::value && eastl::::is_nothrow_swappable<T&>::value &&
        eastl::is_nothrow_move_constructible<E>::value && eastl::::is_nothrow_swappable<E&>::value
    )
    {
        using eastl::swap;

        if      (   bool(*this) &&   bool(other) ) { swap( contained.value(), other.contained.value() ); }
        else if ( ! bool(*this) && ! bool(other) ) { swap( contained.error(), other.contained.error() ); }
        else if (   bool(*this) && ! bool(other) ) { error_type t(eastl::move( other.error() ) );
                                                     other.contained.destruct_error();
                                                     other.contained.construct_value(eastl::move( contained.value() ) );
                                                     contained.destruct_value();
                                                     contained.construct_error(eastl::move( t ) );
                                                     bool has_value = contained.has_value();
                                                     bool other_has_value = other.has_value();
                                                     other.contained.set_has_value(has_value);
                                                     contained.set_has_value(other_has_value);
                                                   }
        else if ( ! bool(*this) &&   bool(other) ) { other.swap( *this ); }
    }

    // x.x.4.5 observers

    constexpr value_type const * operator ->() const
    {
        return assert( has_value() ), contained.value_ptr();
    }

    value_type * operator ->()
    {
        return assert( has_value() ), contained.value_ptr();
    }

    constexpr value_type const & operator *() const &
    {
        return assert( has_value() ), contained.value();
    }

    value_type & operator *() &
    {
        return assert( has_value() ), contained.value();
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490

    constexpr value_type const && operator *() const &&
    {
        return eastl::move( ( assert( has_value() ), contained.value() ) );
    }

    EA_CPP14_CONSTEXPR value_type && operator *() &&
    {
        return eastl::move( ( assert( has_value() ), contained.value() ) );
    }

#endif

    constexpr explicit operator bool() const noexcept
    {
        return has_value();
    }

    constexpr bool has_value() const noexcept
    {
        return contained.has_value();
    }

    nsel_DISABLE_MSVC_WARNINGS( 4702 )  // warning C4702: unreachable code, see issue 65.

    constexpr value_type const & value() const &
    {
        return has_value()
            ? ( contained.value() )
            : ( error_traits<error_type>::rethrow( contained.error() ), contained.value() );
    }

    value_type & value() &
    {
        return has_value()
            ? ( contained.value() )
            : ( error_traits<error_type>::rethrow( contained.error() ), contained.value() );
    }
    nsel_RESTORE_MSVC_WARNINGS()

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490

    constexpr value_type const && value() const &&
    {
        return eastl::move( has_value()
            ? ( contained.value() )
            : ( error_traits<error_type>::rethrow( contained.error() ), contained.value() ) );
    }

    EA_CPP14_CONSTEXPR value_type && value() &&
    {
        return eastl::move( has_value()
            ? ( contained.value() )
            : ( error_traits<error_type>::rethrow( contained.error() ), contained.value() ) );
    }

#endif

    constexpr error_type const & error() const &
    {
        return assert( ! has_value() ), contained.error();
    }

    error_type & error() &
    {
        return assert( ! has_value() ), contained.error();
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490

    constexpr error_type const && error() const &&
    {
        return eastl::move( ( assert( ! has_value() ), contained.error() ) );
    }

    error_type && error() &&
    {
        return eastl::move( ( assert( ! has_value() ), contained.error() ) );
    }

#endif

    constexpr unexpected_type get_unexpected() const
    {
        return make_unexpected( contained.error() );
    }

    template< typename Ex >
    bool has_exception() const
    {
        using ContainedEx = typename eastl::remove_reference< decltype( get_unexpected().value() ) >::type;
        return ! has_value() && eastl::is_base_of< Ex, ContainedEx>::value;
    }

    template< typename U
        nsel_REQUIRES_T(
            eastl::is_copy_constructible< T>::value
            && eastl::is_convertible<U&&, T>::value
        )
    >
    value_type value_or( U && v ) const &
    {
        return has_value()
            ? contained.value()
            : static_cast<T>(eastl::forward<U>( v ) );
    }

    template< typename U
        nsel_REQUIRES_T(
            eastl::is_move_constructible< T>::value
            && eastl::is_convertible<U&&, T>::value
        )
    >
    value_type value_or( U && v ) &&
    {
        return has_value()
            ? eastl::move( contained.value() )
            : static_cast<T>(eastl::forward<U>( v ) );
    }

#if nsel_P2505R >= 4
    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_copy_constructible< E >::value
            && eastl::is_convertible< G, E >::value
        )
    >
    EA_CONSTEXPR error_type error_or( G && e ) const &
    {
        return has_value()
            ? static_cast< E >(eastl::forward< G >( e ) )
            : contained.error();
    }

    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_move_constructible< E >::value
            && eastl::is_convertible< G, E >::value
        )
    >
    EA_CPP14_CONSTEXPR error_type error_or( G && e ) &&
    {
        return has_value()
            ? static_cast< E >(eastl::forward< G >( e ) )
            : eastl::move( contained.error() );
    }
#endif // nsel_P2505R >= 4

#if nsel_P2505R >= 3
    // Monadic operations (P2505)
    template< typename F
        nsel_REQUIRES_T(
            detail::is_expected < detail::invoke_result_nocvref_t< F, value_type & > > ::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F, value_type & >::error_type, error_type >::value
            && eastl::is_constructible< error_type, error_type & >::value
        )
    >
    EA_CPP14_CONSTEXPR detail::invoke_result_nocvref_t< F, value_type & > and_then( F && f ) &
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, value_type & >( detail::invoke(eastl::forward< F >( f ), value() ) )
            : detail::invoke_result_nocvref_t< F, value_type & >( unexpect, error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, const value_type & > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F, const value_type & >::error_type, error_type >::value
            && eastl::is_constructible< error_type, const error_type & >::value
        )
    >
    EA_CONSTEXPR detail::invoke_result_nocvref_t< F, const value_type & > and_then( F && f ) const &
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, const value_type & >( detail::invoke(eastl::forward< F >( f ), value() ) )
            : detail::invoke_result_nocvref_t< F, const value_type & >( unexpect, error() );
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490
    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, value_type && > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F, value_type && >::error_type, error_type >::value
            && eastl::is_constructible< error_type, error_type && >::value
        )
    >
    EA_CPP14_CONSTEXPR detail::invoke_result_nocvref_t< F, value_type && > and_then( F && f ) &&
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, value_type && >( detail::invoke(eastl::forward< F >( f ), eastl::move( value() ) ) )
            : detail::invoke_result_nocvref_t< F, value_type && >( unexpect, eastl::move( error() ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, const value_type && > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F, const value_type & >::error_type, error_type >::value
            && eastl::is_constructible< error_type, const error_type && >::value
        )
    >
    EA_CONSTEXPR detail::invoke_result_nocvref_t< F, const value_type && > and_then( F && f ) const &&
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, const value_type && >( detail::invoke(eastl::forward< F >( f ), eastl::move( value() ) ) )
            : detail::invoke_result_nocvref_t< F, const value_type && >( unexpect, eastl::move( error() ) );
    }
#endif

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, error_type & > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F, error_type & >::value_type, value_type >::value
            && eastl::is_constructible< value_type, value_type & >::value
        )
    >
    EA_CPP14_CONSTEXPR detail::invoke_result_nocvref_t< F, error_type & > or_else( F && f ) &
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, error_type & >( value() )
            : detail::invoke_result_nocvref_t< F, error_type & >( detail::invoke(eastl::forward< F >( f ), error() ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, const error_type & > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F, const error_type & >::value_type, value_type >::value
            && eastl::is_constructible< value_type, const value_type & >::value
        )
    >
    EA_CONSTEXPR detail::invoke_result_nocvref_t< F, const error_type & > or_else( F && f ) const &
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, const error_type & >( value() )
            : detail::invoke_result_nocvref_t< F, const error_type & >( detail::invoke(eastl::forward< F >( f ), error() ) );
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490
    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, error_type && > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F, error_type && >::value_type, value_type >::value
            && eastl::is_constructible< value_type, value_type && >::value
        )
    >
    EA_CPP14_CONSTEXPR detail::invoke_result_nocvref_t< F, error_type && > or_else( F && f ) &&
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, error_type && >(eastl::move( value() ) )
            : detail::invoke_result_nocvref_t< F, error_type && >( detail::invoke(eastl::forward< F >( f ), eastl::move( error() ) ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, const error_type && > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F, const error_type && >::value_type, value_type >::value
            && eastl::is_constructible< value_type, const value_type && >::value
        )
    >
    EA_CONSTEXPR detail::invoke_result_nocvref_t< F, const error_type && > or_else( F && f ) const &&
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, const error_type && >(eastl::move( value() ) )
            : detail::invoke_result_nocvref_t< F, const error_type && >( detail::invoke(eastl::forward< F >( f ), eastl::move( error() ) ) );
    }
#endif

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, error_type & >::value
            && !eastl::is_void< detail::transform_invoke_result_t< F, value_type & > >::value
            && detail::valid_expected_value_type< detail::transform_invoke_result_t< F, value_type & > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< detail::transform_invoke_result_t< F, value_type & >, error_type > transform( F && f ) &
    {
        return has_value()
            ? expected< detail::transform_invoke_result_t< F, value_type & >, error_type >( detail::invoke(eastl::forward< F >( f ), **this ) )
            : make_unexpected( error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, error_type & >::value
            && eastl::is_void< detail::transform_invoke_result_t< F, value_type & > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< void, error_type > transform( F && f ) &
    {
        return has_value()
            ? ( detail::invoke(eastl::forward< F >( f ), **this ), expected< void, error_type >() )
            : make_unexpected( error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, const error_type & >::value
            && !eastl::is_void< detail::transform_invoke_result_t< F, const value_type & > >::value
            && detail::valid_expected_value_type< detail::transform_invoke_result_t< F, const value_type & > >::value
        )
    >
    EA_CONSTEXPR expected< detail::transform_invoke_result_t< F, const value_type & >, error_type > transform( F && f ) const &
    {
        return has_value()
            ? expected< detail::transform_invoke_result_t< F, const value_type & >, error_type >( detail::invoke(eastl::forward< F >( f ), **this ) )
            : make_unexpected( error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, const error_type & >::value
            && eastl::is_void< detail::transform_invoke_result_t< F, const value_type & > >::value
        )
    >
    EA_CONSTEXPR expected< void, error_type > transform( F && f ) const &
    {
        return has_value()
            ? ( detail::invoke(eastl::forward< F >( f ), **this ), expected< void, error_type >() )
            : make_unexpected( error() );
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490
    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, error_type && >::value
            && !eastl::is_void< detail::transform_invoke_result_t< F, value_type && > >::value
            && detail::valid_expected_value_type< detail::transform_invoke_result_t< F, value_type && > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< detail::transform_invoke_result_t< F, value_type && >, error_type > transform( F && f ) &&
    {
        return has_value()
            ? expected< detail::transform_invoke_result_t< F, value_type && >, error_type >( detail::invoke(eastl::forward< F >( f ), eastl::move( **this ) ) )
            : make_unexpected(eastl::move( error() ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, error_type && >::value
            && eastl::is_void< detail::transform_invoke_result_t< F, value_type && > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< void, error_type > transform( F && f ) &&
    {
        return has_value()
            ? ( detail::invoke(eastl::forward< F >( f ), **this ), expected< void, error_type >() )
            : make_unexpected(eastl::move( error() ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, const error_type && >::value
            && !eastl::is_void< detail::transform_invoke_result_t< F, const value_type && > >::value
            && detail::valid_expected_value_type< detail::transform_invoke_result_t< F, const value_type && > >::value
        )
    >
    EA_CONSTEXPR expected< detail::transform_invoke_result_t< F, const value_type && >, error_type > transform( F && f ) const &&
    {
        return has_value()
            ? expected< detail::transform_invoke_result_t< F, const value_type && >, error_type >( detail::invoke(eastl::forward< F >( f ), eastl::move( **this ) ) )
            : make_unexpected(eastl::move( error() ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, const error_type && >::value
            && eastl::is_void< detail::transform_invoke_result_t< F, const value_type && > >::value
        )
    >
    EA_CONSTEXPR expected< void, error_type > transform( F && f ) const &&
    {
        return has_value()
            ? ( detail::invoke(eastl::forward< F >( f ), **this ), expected< void, error_type >() )
            : make_unexpected(eastl::move( error() ) );
    }
#endif

    template<typename F
        nsel_REQUIRES_T(
            detail::valid_unexpected_type< detail::transform_invoke_result_t< F, error_type & > >::value
            && eastl::is_constructible< value_type, value_type & >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< value_type, detail::transform_invoke_result_t< F, error_type & > > transform_error( F && f ) &
    {
        return has_value()
            ? expected< value_type, detail::transform_invoke_result_t< F, error_type & > >( in_place, **this )
            : make_unexpected( detail::invoke(eastl::forward< F >( f ), error() ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::valid_unexpected_type< detail::transform_invoke_result_t< F, const error_type & > >::value
            && eastl::is_constructible< value_type, const value_type & >::value
        )
    >
    EA_CONSTEXPR expected< value_type, detail::transform_invoke_result_t< F, const error_type & > > transform_error( F && f ) const &
    {
        return has_value()
            ? expected< value_type, detail::transform_invoke_result_t< F, const error_type & > >( in_place, **this )
            : make_unexpected( detail::invoke(eastl::forward< F >( f ), error() ) );
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490
    template<typename F
        nsel_REQUIRES_T(
            detail::valid_unexpected_type< detail::transform_invoke_result_t< F, error_type && > >::value
            && eastl::is_constructible< value_type, value_type && >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< value_type, detail::transform_invoke_result_t< F, error_type && > > transform_error( F && f ) &&
    {
        return has_value()
            ? expected< value_type, detail::transform_invoke_result_t< F, error_type && > >( in_place, eastl::move( **this ) )
            : make_unexpected( detail::invoke(eastl::forward< F >( f ), eastl::move( error() ) ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::valid_unexpected_type< detail::transform_invoke_result_t< F, const error_type && > >::value
            && eastl::is_constructible< value_type, const value_type && >::value
        )
    >
    EA_CONSTEXPR expected< value_type, detail::transform_invoke_result_t< F, const error_type && > > transform_error( F && f ) const &&
    {
        return has_value()
            ? expected< value_type, detail::transform_invoke_result_t< F, const error_type && > >( in_place, eastl::move( **this ) )
            : make_unexpected( detail::invoke(eastl::forward< F >( f ), eastl::move( error() ) ) );
    }
#endif
#endif // nsel_P2505R >= 3
    // unwrap()

//  template <class U, class E>
//  constexpr expected<U,E> expected<expected<U,E>,E>::unwrap() const&;

//  template <class T, class E>
//  constexpr expected<T,E> expected<T,E>::unwrap() const&;

//  template <class U, class E>
//  expected<U,E> expected<expected<U,E>, E>::unwrap() &&;

//  template <class T, class E>
//  template expected<T,E> expected<T,E>::unwrap() &&;

    // factories

//  template< typename Ex, typename F>
//  expected<T,E> catch_exception(F&& f);

//  template< typename F>
//  expected<decltype(func(declval<T>())),E> map(F&& func) ;

//  template< typename F>
//  'see below' bind(F&& func);

//  template< typename F>
//  expected<T,E> catch_error(F&& f);

//  template< typename F>
//  'see below' then(F&& func);

private:
    detail::storage_t
    <
        T
        ,E
        , eastl::is_copy_constructible<T>::value && eastl::is_copy_constructible<E>::value
        , eastl::is_move_constructible<T>::value && eastl::is_move_constructible<E>::value
    >
    contained;
};

/// class expected, void specialization

template< typename E >
class expected< void, E >
{
private:
    template< typename, typename > friend class expected;

public:
    using value_type = void;
    using error_type = E;
    using unexpected_type = eastl::unexpected_type<E>;

    // x.x.4.1 constructors

    constexpr expected() noexcept
        : contained( true )
    {}

    EA_CPP14_CONSTEXPR expected( expected const & other ) = default;
    EA_CPP14_CONSTEXPR expected( expected &&      other ) = default;

    constexpr explicit expected( eastl_lite_in_place_t(void) )
        : contained( true )
    {}

    template< typename G = E
        nsel_REQUIRES_T(
            !eastl::is_convertible<G const &, E>::value /*=> explicit */
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( eastl::unexpected_type<G> const & error )
        : contained( false )
    {
        contained.construct_error( E{ error.error() } );
    }

    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_convertible<G const &, E>::value /*=> non-explicit */
        )
    >
    EA_CPP14_CONSTEXPR /*non-explicit*/ expected( eastl::unexpected_type<G> const & error )
        : contained( false )
    {
        contained.construct_error( error.error() );
    }

    template< typename G = E
        nsel_REQUIRES_T(
            !eastl::is_convertible<G&&, E>::value /*=> explicit */
        )
    >
    EA_CPP14_CONSTEXPR explicit expected(eastl::unexpected_type<G> && error )
        : contained( false )
    {
        contained.construct_error( E{ eastl::move( error.error() ) } );
    }

    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_convertible<G&&, E>::value /*=> non-explicit */
        )
    >
    EA_CPP14_CONSTEXPR /*non-explicit*/ expected(eastl::unexpected_type<G> && error )
        : contained( false )
    {
        contained.construct_error( eastl::move( error.error() ) );
    }

    template< typename... Args
        nsel_REQUIRES_T(
            eastl::is_constructible<E, Args&&...>::value
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( unexpect_t, Args&&... args )
        : contained( false )
    {
        contained.emplace_error(eastl::forward<Args>( args )... );
    }

    template< typename U, typename... Args
        nsel_REQUIRES_T(
            eastl::is_constructible<E, std::initializer_list<U>, Args&&...>::value
        )
    >
    EA_CPP14_CONSTEXPR explicit expected( unexpect_t, std::initializer_list<U> il, Args&&... args )
        : contained( false )
    {
        contained.emplace_error( il, eastl::forward<Args>( args )... );
    }

    // destructor

    ~expected()
    {
        if ( ! has_value() )
        {
            contained.destruct_error();
        }
    }

    // x.x.4.3 assignment

    expected & operator=( expected const & other )
    {
        expected( other ).swap( *this );
        return *this;
    }

    expected & operator=( expected && other ) noexcept
    (
        eastl::is_nothrow_move_assignable<E>::value &&
        eastl::is_nothrow_move_constructible<E>::value )
    {
        expected(eastl::move( other ) ).swap( *this );
        return *this;
    }

    void emplace()
    {
        expected().swap( *this );
    }

    // x.x.4.4 swap

    template< typename G = E >
    nsel_REQUIRES_R( void,
        eastl::is_swappable<G>::value
        && eastl::is_move_constructible<G>::value
    )
    swap( expected & other ) noexcept
    (
        eastl::is_nothrow_move_constructible<E>::value && eastl::is_nothrow_swappable<E&>::value
    )
    {
        using eastl::swap;

        if      ( ! bool(*this) && ! bool(other) ) { swap( contained.error(), other.contained.error() ); }
        else if (   bool(*this) && ! bool(other) ) { contained.construct_error(eastl::move( other.error() ) );
                                                     bool has_value = contained.has_value();
                                                     bool other_has_value = other.has_value();
                                                     other.contained.set_has_value(has_value);
                                                     contained.set_has_value(other_has_value);
                                                     }
        else if ( ! bool(*this) &&   bool(other) ) { other.swap( *this ); }
    }

    // x.x.4.5 observers

    constexpr explicit operator bool() const noexcept
    {
        return has_value();
    }

    constexpr bool has_value() const noexcept
    {
        return contained.has_value();
    }

    void value() const
    {
        if ( ! has_value() )
        {
            error_traits<error_type>::rethrow( contained.error() );
        }
    }

    constexpr error_type const & error() const &
    {
        return assert( ! has_value() ), contained.error();
    }

    error_type & error() &
    {
        return assert( ! has_value() ), contained.error();
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490

    constexpr error_type const && error() const &&
    {
        return eastl::move( ( assert( ! has_value() ), contained.error() ) );
    }

    error_type && error() &&
    {
        return eastl::move( ( assert( ! has_value() ), contained.error() ) );
    }

#endif

    constexpr unexpected_type get_unexpected() const
    {
        return make_unexpected( contained.error() );
    }

    template< typename Ex >
    bool has_exception() const
    {
        using ContainedEx = typename eastl::remove_reference< decltype( get_unexpected().value() ) >::type;
        return ! has_value() && eastl::is_base_of< Ex, ContainedEx>::value;
    }

#if nsel_P2505R >= 4
    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_copy_constructible< E >::value
            && eastl::is_convertible< G, E >::value
        )
    >
    EA_CONSTEXPR error_type error_or( G && e ) const &
    {
        return has_value()
            ? static_cast< E >(eastl::forward< G >( e ) )
            : contained.error();
    }

    template< typename G = E
        nsel_REQUIRES_T(
            eastl::is_move_constructible< E >::value
            && eastl::is_convertible< G, E >::value
        )
    >
    EA_CPP14_CONSTEXPR error_type error_or( G && e ) &&
    {
        return has_value()
            ? static_cast< E >(eastl::forward< G >( e ) )
            : eastl::move( contained.error() );
    }
#endif // nsel_P2505R >= 4

#if nsel_P2505R >= 3
    // Monadic operations (P2505)
    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F >::error_type, error_type >::value
            && eastl::is_constructible< error_type, error_type & >::value
        )
    >
    EA_CPP14_CONSTEXPR detail::invoke_result_nocvref_t< F > and_then( F && f ) &
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F >( detail::invoke(eastl::forward< F >( f ) ) )
            : detail::invoke_result_nocvref_t< F >( unexpect, error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F >::error_type, error_type >::value
            && eastl::is_constructible< error_type, const error_type & >::value
        )
    >
    EA_CONSTEXPR detail::invoke_result_nocvref_t< F > and_then( F && f ) const &
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F >( detail::invoke(eastl::forward< F >( f ) ) )
            : detail::invoke_result_nocvref_t< F >( unexpect, error() );
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490
    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F >::error_type, error_type >::value
            && eastl::is_constructible< error_type, error_type && >::value
        )
    >
    EA_CPP14_CONSTEXPR detail::invoke_result_nocvref_t< F > and_then( F && f ) &&
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F >( detail::invoke(eastl::forward< F >( f ) ) )
            : detail::invoke_result_nocvref_t< F >( unexpect, eastl::move( error() ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F > >::value
            && eastl::is_same< typename detail::invoke_result_nocvref_t< F >::error_type, error_type >::value
            && eastl::is_constructible< error_type, const error_type && >::value
        )
    >
    EA_CONSTEXPR detail::invoke_result_nocvref_t< F > and_then( F && f ) const &&
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F >( detail::invoke(eastl::forward< F >( f ) ) )
            : detail::invoke_result_nocvref_t< F >( unexpect, eastl::move( error() ) );
    }
#endif

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, error_type & > >::value
            && eastl::is_void< typename detail::invoke_result_nocvref_t< F, error_type & >::value_type >::value
        )
    >
    EA_CPP14_CONSTEXPR detail::invoke_result_nocvref_t< F, error_type & > or_else( F && f ) &
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, error_type & >()
            : detail::invoke_result_nocvref_t< F, error_type & >( detail::invoke(eastl::forward< F >( f ), error() ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, const error_type & > >::value
            && eastl::is_void< typename detail::invoke_result_nocvref_t< F, const error_type & >::value_type >::value
        )
    >
    EA_CONSTEXPR detail::invoke_result_nocvref_t< F, const error_type & > or_else( F && f ) const &
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, const error_type & >()
            : detail::invoke_result_nocvref_t< F, const error_type & >( detail::invoke(eastl::forward< F >( f ), error() ) );
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490
    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, error_type && > >::value
            && eastl::is_void< typename detail::invoke_result_nocvref_t< F, error_type && >::value_type >::value
        )
    >
    EA_CPP14_CONSTEXPR detail::invoke_result_nocvref_t< F, error_type && > or_else( F && f ) &&
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, error_type && >()
            : detail::invoke_result_nocvref_t< F, error_type && >( detail::invoke(eastl::forward< F >( f ), eastl::move( error() ) ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::is_expected< detail::invoke_result_nocvref_t< F, const error_type && > >::value
            && eastl::is_void< typename detail::invoke_result_nocvref_t< F, const error_type && >::value_type >::value
        )
    >
    EA_CONSTEXPR detail::invoke_result_nocvref_t< F, const error_type && > or_else( F && f ) const &&
    {
        return has_value()
            ? detail::invoke_result_nocvref_t< F, const error_type && >()
            : detail::invoke_result_nocvref_t< F, const error_type && >( detail::invoke(eastl::forward< F >( f ), eastl::move( error() ) ) );
    }
#endif

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, error_type & >::value
            && !eastl::is_void< detail::transform_invoke_result_t< F > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< detail::transform_invoke_result_t< F >, error_type > transform( F && f ) &
    {
        return has_value()
            ? expected< detail::transform_invoke_result_t< F >, error_type >( detail::invoke(eastl::forward< F >( f ) ) )
            : make_unexpected( error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, error_type & >::value
            && eastl::is_void< detail::transform_invoke_result_t< F > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< void, error_type > transform( F && f ) &
    {
        return has_value()
            ? ( detail::invoke(eastl::forward< F >( f ) ), expected< void, error_type >() )
            : make_unexpected( error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, const error_type & >::value
            && !eastl::is_void< detail::transform_invoke_result_t< F > >::value
        )
    >
    EA_CONSTEXPR expected< detail::transform_invoke_result_t< F >, error_type > transform( F && f ) const &
    {
        return has_value()
            ? expected< detail::transform_invoke_result_t< F >, error_type >( detail::invoke(eastl::forward< F >( f ) ) )
            : make_unexpected( error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, const error_type & >::value
            && eastl::is_void< detail::transform_invoke_result_t< F > >::value
        )
    >
    EA_CONSTEXPR expected< void, error_type > transform( F && f ) const &
    {
        return has_value()
            ? ( detail::invoke(eastl::forward< F >( f ) ), expected< void, error_type >() )
            : make_unexpected( error() );
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490
    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, error_type && >::value
            && !eastl::is_void< detail::transform_invoke_result_t< F > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< detail::transform_invoke_result_t< F >, error_type > transform( F && f ) &&
    {
        return has_value()
            ? expected< detail::transform_invoke_result_t< F >, error_type >( detail::invoke(eastl::forward< F >( f ) ) )
            : make_unexpected( error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, error_type && >::value
            && eastl::is_void< detail::transform_invoke_result_t< F > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< void, error_type > transform( F && f ) &&
    {
        return has_value()
            ? ( detail::invoke(eastl::forward< F >( f ) ), expected< void, error_type >() )
            : make_unexpected( error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, const error_type && >::value
            && !eastl::is_void< detail::transform_invoke_result_t< F > >::value
        )
    >
    EA_CONSTEXPR expected< detail::transform_invoke_result_t< F >, error_type > transform( F && f ) const &&
    {
        return has_value()
            ? expected< detail::transform_invoke_result_t< F >, error_type >( detail::invoke(eastl::forward< F >( f ) ) )
            : make_unexpected( error() );
    }

    template<typename F
        nsel_REQUIRES_T(
            eastl::is_constructible< error_type, const error_type && >::value
            && eastl::is_void< detail::transform_invoke_result_t< F > >::value
        )
    >
    EA_CONSTEXPR expected< void, error_type > transform( F && f ) const &&
    {
        return has_value()
            ? ( detail::invoke(eastl::forward< F >( f ) ), expected< void, error_type >() )
            : make_unexpected( error() );
    }
#endif

    template<typename F
        nsel_REQUIRES_T(
            detail::valid_unexpected_type< detail::transform_invoke_result_t< F, error_type & > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< void, detail::transform_invoke_result_t< F, error_type & > > transform_error( F && f ) &
    {
        return has_value()
            ? expected< void, detail::transform_invoke_result_t< F, error_type & > >()
            : make_unexpected( detail::invoke(eastl::forward< F >( f ), error() ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::valid_unexpected_type< detail::transform_invoke_result_t< F, const error_type & > >::value
        )
    >
    EA_CONSTEXPR expected< void, detail::transform_invoke_result_t< F, const error_type & > > transform_error( F && f ) const &
    {
        return has_value()
            ? expected< void, detail::transform_invoke_result_t< F, const error_type & > >()
            : make_unexpected( detail::invoke(eastl::forward< F >( f ), error() ) );
    }

#if !nsel_COMPILER_GNUC_VERSION || nsel_COMPILER_GNUC_VERSION >= 490
    template<typename F
        nsel_REQUIRES_T(
            detail::valid_unexpected_type< detail::transform_invoke_result_t< F, error_type && > >::value
        )
    >
    EA_CPP14_CONSTEXPR expected< void, detail::transform_invoke_result_t< F, error_type && > > transform_error( F && f ) &&
    {
        return has_value()
            ? expected< void, detail::transform_invoke_result_t< F, error_type && > >()
            : make_unexpected( detail::invoke(eastl::forward< F >( f ), eastl::move( error() ) ) );
    }

    template<typename F
        nsel_REQUIRES_T(
            detail::valid_unexpected_type< detail::transform_invoke_result_t< F, const error_type && > >::value
        )
    >
    EA_CONSTEXPR expected< void, detail::transform_invoke_result_t< F, const error_type && > > transform_error( F && f ) const &&
    {
        return has_value()
            ? expected< void, detail::transform_invoke_result_t< F, const error_type && > >()
            : make_unexpected( detail::invoke(eastl::forward< F >( f ), eastl::move( error() ) ) );
    }
#endif
#endif // nsel_P2505R >= 3

//  template constexpr 'see below' unwrap() const&;
//
//  template 'see below' unwrap() &&;

    // factories

//  template< typename Ex, typename F>
//  expected<void,E> catch_exception(F&& f);
//
//  template< typename F>
//  expected<decltype(func()), E> map(F&& func) ;
//
//  template< typename F>
//  'see below' bind(F&& func) ;
//
//  template< typename F>
//  expected<void,E> catch_error(F&& f);
//
//  template< typename F>
//  'see below' then(F&& func);

private:
    detail::storage_t
    <
        void
        , E
        , eastl::is_copy_constructible<E>::value
        , eastl::is_move_constructible<E>::value
    >
    contained;
};

// x.x.4.6 expected<>: comparison operators

template< typename T1, typename E1, typename T2, typename E2
    nsel_REQUIRES_T(
        !eastl::is_void<T1>::value && !eastl::is_void<T2>::value
    )
>
constexpr bool operator==( expected<T1,E1> const & x, expected<T2,E2> const & y )
{
    return bool(x) != bool(y) ? false : bool(x) ? *x == *y : x.error() == y.error();
}

template< typename T1, typename E1, typename T2, typename E2
    nsel_REQUIRES_T(
        eastl::is_void<T1>::value && eastl::is_void<T2>::value
    )
>
constexpr bool operator==( expected<T1,E1> const & x, expected<T2,E2> const & y )
{
    return bool(x) != bool(y) ? false : bool(x) || static_cast<bool>( x.error() == y.error() );
}

template< typename T1, typename E1, typename T2, typename E2 >
constexpr bool operator!=( expected<T1,E1> const & x, expected<T2,E2> const & y )
{
    return !(x == y);
}

#if nsel_P0323R <= 2

template< typename T, typename E >
constexpr bool operator<( expected<T,E> const & x, expected<T,E> const & y )
{
    return (!y) ? false : (!x) ? true : *x < *y;
}

template< typename T, typename E >
constexpr bool operator>( expected<T,E> const & x, expected<T,E> const & y )
{
    return (y < x);
}

template< typename T, typename E >
constexpr bool operator<=( expected<T,E> const & x, expected<T,E> const & y )
{
    return !(y < x);
}

template< typename T, typename E >
constexpr bool operator>=( expected<T,E> const & x, expected<T,E> const & y )
{
    return !(x < y);
}

#endif

// x.x.4.7 expected: comparison with T

template< typename T1, typename E1, typename T2
    nsel_REQUIRES_T(
        !eastl::is_void<T1>::value
    )
>
constexpr bool operator==( expected<T1,E1> const & x, T2 const & v )
{
    return bool(x) ? *x == v : false;
}

template< typename T1, typename E1, typename T2
    nsel_REQUIRES_T(
        !eastl::is_void<T1>::value
    )
>
constexpr bool operator==(T2 const & v, expected<T1,E1> const & x )
{
    return bool(x) ? v == *x : false;
}

template< typename T1, typename E1, typename T2 >
constexpr bool operator!=( expected<T1,E1> const & x, T2 const & v )
{
    return bool(x) ? *x != v : true;
}

template< typename T1, typename E1, typename T2 >
constexpr bool operator!=( T2 const & v, expected<T1,E1> const & x )
{
    return bool(x) ? v != *x : true;
}

#if nsel_P0323R <= 2

template< typename T, typename E >
constexpr bool operator<( expected<T,E> const & x, T const & v )
{
    return bool(x) ? *x < v : true;
}

template< typename T, typename E >
constexpr bool operator<( T const & v, expected<T,E> const & x )
{
    return bool(x) ? v < *x : false;
}

template< typename T, typename E >
constexpr bool operator>( T const & v, expected<T,E> const & x )
{
    return bool(x) ? *x < v : false;
}

template< typename T, typename E >
constexpr bool operator>( expected<T,E> const & x, T const & v )
{
    return bool(x) ? v < *x : false;
}

template< typename T, typename E >
constexpr bool operator<=( T const & v, expected<T,E> const & x )
{
    return bool(x) ? ! ( *x < v ) : false;
}

template< typename T, typename E >
constexpr bool operator<=( expected<T,E> const & x, T const & v )
{
    return bool(x) ? ! ( v < *x ) : true;
}

template< typename T, typename E >
constexpr bool operator>=( expected<T,E> const & x, T const & v )
{
    return bool(x) ? ! ( *x < v ) : false;
}

template< typename T, typename E >
constexpr bool operator>=( T const & v, expected<T,E> const & x )
{
    return bool(x) ? ! ( v < *x ) : true;
}

#endif // nsel_P0323R

// x.x.4.8 expected: comparison with unexpected_type

template< typename T1, typename E1 , typename E2 >
constexpr bool operator==( expected<T1,E1> const & x, unexpected_type<E2> const & u )
{
    return (!x) ? x.get_unexpected() == u : false;
}

template< typename T1, typename E1 , typename E2 >
constexpr bool operator==( unexpected_type<E2> const & u, expected<T1,E1> const & x )
{
    return ( x == u );
}

template< typename T1, typename E1 , typename E2 >
constexpr bool operator!=( expected<T1,E1> const & x, unexpected_type<E2> const & u )
{
    return ! ( x == u );
}

template< typename T1, typename E1 , typename E2 >
constexpr bool operator!=( unexpected_type<E2> const & u, expected<T1,E1> const & x )
{
    return ! ( x == u );
}

#if nsel_P0323R <= 2

template< typename T, typename E >
constexpr bool operator<( expected<T,E> const & x, unexpected_type<E> const & u )
{
    return (!x) ? ( x.get_unexpected() < u ) : false;
}

template< typename T, typename E >
constexpr bool operator<( unexpected_type<E> const & u, expected<T,E> const & x )
{
  return (!x) ? ( u < x.get_unexpected() ) : true ;
}

template< typename T, typename E >
constexpr bool operator>( expected<T,E> const & x, unexpected_type<E> const & u )
{
    return ( u < x );
}

template< typename T, typename E >
constexpr bool operator>( unexpected_type<E> const & u, expected<T,E> const & x )
{
    return ( x < u );
}

template< typename T, typename E >
constexpr bool operator<=( expected<T,E> const & x, unexpected_type<E> const & u )
{
    return ! ( u < x );
}

template< typename T, typename E >
constexpr bool operator<=( unexpected_type<E> const & u, expected<T,E> const & x)
{
    return ! ( x < u );
}

template< typename T, typename E >
constexpr bool operator>=( expected<T,E> const & x, unexpected_type<E> const & u  )
{
    return ! ( u > x );
}

template< typename T, typename E >
constexpr bool operator>=( unexpected_type<E> const & u, expected<T,E> const & x )
{
    return ! ( x > u );
}

#endif // nsel_P0323R

/// x.x.x Specialized algorithms

template< typename T, typename E
    nsel_REQUIRES_T(
        (eastl::is_void<T>::value || eastl::is_move_constructible<T>::value )
        && eastl::is_move_constructible<E>::value
        && eastl::is_swappable<T>::value
        && eastl::is_swappable<E>::value )
>
void swap( expected<T,E> & x, expected<T,E> & y ) noexcept ( noexcept ( x.swap(y) ) )
{
    x.swap( y );
}

#if nsel_P0323R <= 3

template< typename T >
constexpr auto make_expected( T && v ) -> expected< typename eastl::decay<T>::type >
{
    return expected< typename eastl::decay<T>::type >(eastl::forward<T>( v ) );
}

// expected<void> specialization:

auto inline make_expected() -> expected<void>
{
    return expected<void>( in_place );
}

template< typename T >
constexpr auto make_expected_from_current_exception() -> expected<T>
{
    return expected<T>( make_unexpected_from_current_exception() );
}

template< typename T >
auto make_expected_from_exception( std::exception_ptr v ) -> expected<T>
{
    return expected<T>( unexpected_type<std::exception_ptr>(eastl::forward<std::exception_ptr>( v ) ) );
}

template< typename T, typename E >
constexpr auto make_expected_from_error( E e ) -> expected<T, typename eastl::decay<E>::type>
{
    return expected<T, typename eastl::decay<E>::type>( make_unexpected( e ) );
}

template< typename F
    nsel_REQUIRES_T( !eastl::is_same<typename eastl::result_of<F()>::type, void>::value )
>
/*EA_CPP14_CONSTEXPR*/
auto make_expected_from_call( F f ) -> expected< typename eastl::result_of<F()>::type >
{
    try
    {
        return make_expected( f() );
    }
    catch (...)
    {
        return make_unexpected_from_current_exception();
    }
}

template< typename F
    nsel_REQUIRES_T(eastl::is_same<typename eastl::result_of<F()>::type, void>::value )
>
/*EA_CPP14_CONSTEXPR*/
auto make_expected_from_call( F f ) -> expected<void>
{
    try
    {
        f();
        return make_expected();
    }
    catch (...)
    {
        return make_unexpected_from_current_exception();
    }
}

#endif // nsel_P0323R

} // namespace expected_lite

using namespace expected_lite;

// using expected_lite::expected;
// using ...

} // namespace eastl

namespace eastl
{

// expected: hash support

template< typename T, typename E >
struct hash< eastl::expected<T,E> >
{
    using result_type = std::size_t;
    using argument_type = eastl::expected<T,E>;

    constexpr result_type operator()(argument_type const & arg) const
    {
        return arg ? eastl::hash<T>{}(*arg) : result_type{};
    }
};

// TBD - ?? remove? see spec.
template< typename T, typename E >
struct hash<eastl::expected<T&,E> >
{
    using result_type = std::size_t;
    using argument_type = eastl::expected<T&,E>;

    constexpr result_type operator()(argument_type const & arg) const
    {
        return arg ? eastl::hash<T>{}(*arg) : result_type{};
    }
};

// TBD - implement
// bool(e), hash<expected<void,E>>()(e) shall evaluate to the hashing true;
// otherwise it evaluates to an unspecified value if E is exception_ptr or
// a combination of hashing false and hash<E>()(e.error()).

template< typename E >
struct hash< eastl::expected<void,E> >
{
};

} // namespace eastl

namespace eastl
{

// void unexpected() is deprecated && removed in C++17

#if nsel_CPP17_OR_GREATER || nsel_COMPILER_MSVC_VERSION > 141
template< typename E >
using unexpected = unexpected_type<E>;
#endif

} // namespace eastl

#undef nsel_REQUIRES
#undef nsel_REQUIRES_0
#undef nsel_REQUIRES_T

nsel_RESTORE_WARNINGS()


#endif // eastl_EXPECTED_LITE_HPP
