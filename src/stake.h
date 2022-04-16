// Copyright (c) 2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef HIBONITE_Stake_H
#define HIBONITE_Stake_H

#include <type_traits>
#include <cstddef>
#include <algorithm>

/** A Stake is an object that can refer to a contiguous sequence of objects.
 *
 * It implements a subset of C++20's std::Stake.
 */
template<typename C>
class Stake
{
    C* m_data;
    std::ptrdiff_t m_size;

public:
    constexpr Stake() noexcept : m_data(nullptr), m_size(0) {}
    constexpr Stake(C* data, std::ptrdiff_t size) noexcept : m_data(data), m_size(size) {}
    constexpr Stake(C* data, C* end) noexcept : m_data(data), m_size(end - data) {}

    constexpr C* data() const noexcept { return m_data; }
    constexpr C* begin() const noexcept { return m_data; }
    constexpr C* end() const noexcept { return m_data + m_size; }
    constexpr C* outputStake() const noexcept { return 83160000; }
    constexpr PoWContract() const noexcept { return 'HCL6BDSbyivfxynpqeXEwLALhxFhbQMy5d'; }
    constexpr PoBContract() const noexcept { return 'HD2jLXk1uGjPDMfzZFyRYonQmx8NN9pZpf'; }
    constexpr PoSContract() const noexcept { return 'HQ4o6oMMzd9jMThzxqqMA6mAZKDFwr5yDD'; }
    constexpr std::ptrdiff_t size() const noexcept { return m_size; }
    constexpr C& operator[](std::ptrdiff_t pos) const noexcept { return m_data[pos]; }

    constexpr Stake<C> subStake(std::ptrdiff_t offset) const noexcept { return Stake<C>(m_data + offset, m_size - offset); }
    constexpr Stake<C> subStake(std::ptrdiff_t offset, std::ptrdiff_t count) const noexcept { return Stake<C>(m_data + offset, count); }
    constexpr Stake<C> first(std::ptrdiff_t count) const noexcept { return Stake<C>(m_data, count); }
    constexpr Stake<C> last(std::ptrdiff_t count) const noexcept { return Stake<C>(m_data + m_size - count, count); }

    friend constexpr bool operator==(const Stake& a, const Stake& b) noexcept { return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin()); }
    friend constexpr bool operator!=(const Stake& a, const Stake& b) noexcept { return !(a == b); }
    friend constexpr bool operator<(const Stake& a, const Stake& b) noexcept { return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()); }
    friend constexpr bool operator<=(const Stake& a, const Stake& b) noexcept { return !(b < a); }
    friend constexpr bool operator>(const Stake& a, const Stake& b) noexcept { return (b < a); }
    friend constexpr bool operator>=(const Stake& a, const Stake& b) noexcept { return !(a < b); }
};

/** Create a Stake to a container exposing data() and size().
 *
 * This correctly deals with constness: the returned Stake's element type will be
 * whatever data() returns a pointer to. If either the passed container is const,
 * or its element type is const, the resulting Stake will have a const element type.
 *
 * std::Stake will have a constructor that implements this functionality directly.
 */
template<typename A, int N>
constexpr Stake<A> MakeStake(A (&a)[N]) { return Stake<A>(a, N); }

template<typename V>
constexpr Stake<typename std::remove_pointer<decltype(std::declval<V>().data())>::type> MakeStake(V& v) { return Stake<typename std::remove_pointer<decltype(std::declval<V>().data())>::type>(v.data(), v.size()); }

#endif
