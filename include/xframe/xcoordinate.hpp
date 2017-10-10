/***************************************************************************
* Copyright (c) 2017, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XFRAME_XCOORDINATE_HPP
#define XFRAME_XCOORDINATE_HPP

#include <unordered_map>
#include "xtl/xvariant.hpp"
#include "xaxis.hpp"

namespace xf
{

    /***************
     * xcoordinate *
     ***************/

    template <class K, class S, class... L>
    class xcoordinate
    {
    public:

        using self_type = xcoordinate<K, S, L...>;
        using axis_type = xtl::variant<xaxis<L, S>...>;
        using map_type = std::unordered_map<K, axis_type>;
        using key_type = K;
        using mapped_type = axis_type;
        using index_type = S;
        using value_type = typename map_type::value_type;
        using reference = typename map_type::reference;
        using const_reference = typename map_type::const_reference;
        using pointer = typename map_type::pointer;
        using const_pointer = typename map_type::const_pointer;
        using size_type = typename map_type::size_type;
        using difference_type = typename map_type::difference_type;
        using iterator = typename map_type::iterator;
        using const_iterator = typename map_type::const_iterator;

        explicit xcoordinate(const map_type& axes);
        explicit xcoordinate(map_type&& axes);
        template <class... LB>
        explicit xcoordinate(std::pair<K, xaxis<LB, S>>... axes);

        bool empty() const;
        size_type size() const;
        
        bool contains(const key_type& key) const;
        const mapped_type& operator[](const key_type& key) const;

        template <class KB, class LB>
        index_type operator[](const std::pair<KB, LB>& key) const;

        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;

        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        bool operator==(const self_type& rhs) const noexcept;
        bool operator!=(const self_type& rhs) const noexcept;

    private:
        
        template <class LB1, class... LB>
        void insert_impl(std::pair<K, xaxis<LB1, S>> axis, std::pair<K, xaxis<LB, S>>... axes);
        void insert_impl();

        map_type m_coordinate;
    };

    template <class K, class S, class... L>
    xcoordinate<K, S, L...> coordinate(const std::unordered_map<K, xtl::variant<xaxis<L, S>...>>& axes);

    template <class K, class S, class... L>
    xcoordinate<K, S, L...> coordinate(std::unordered_map<K, xtl::variant<xaxis<L, S>...>>&& axes);

    template <class K, class S, class... L>
    xcoordinate<K, S, L...> coordinate(std::pair<K, xaxis<L, S>>... axes);

    /******************************
     * xcoordinate implementation *
     ******************************/

    template <class K, class S, class... L>
    xcoordinate<K, S, L...>::xcoordinate(const map_type& axes)
        : m_coordinate(axes)
    {
    }

    template <class K, class S, class... L>
    xcoordinate<K, S, L...>::xcoordinate(map_type&& axes)
        : m_coordinate(std::move(axes))
    {
    }

    template <class K, class S, class... L>
    template <class... LB>
    inline xcoordinate<K, S, L...>::xcoordinate(std::pair<K, xaxis<LB, S>>... axes)
    {
        insert_impl(std::move(axes)...);
    }
    
    template <class K, class S, class... L>
    inline bool xcoordinate<K, S, L...>::empty() const
    {
        return m_coordinate.empty();
    }

    template <class K, class S, class... L>
    inline auto xcoordinate<K, S, L...>::size() const -> size_type
    {
        return m_coordinate.size();
    }

    template <class K, class S, class... L>
    inline bool xcoordinate<K, S, L...>::contains(const key_type& key) const
    {
        return m_coordinate.find(key) != m_coordinate.end();
    }

    template <class K, class S, class... L>
    inline auto xcoordinate<K, S, L...>::operator[](const key_type& key) const -> const mapped_type&
    {
        return m_coordinate.at(key);
    }

    template <class K, class S, class... L>
    template <class KB, class LB>
    inline auto xcoordinate<K, S, L...>::operator[](const std::pair<KB, LB>& key) const -> index_type
    {
        return xtl::get<xaxis<LB, S>>((*this)[key.first])[key.second];
    }

    template <class K, class S, class... L>
    inline auto xcoordinate<K, S, L...>::begin() const noexcept -> const_iterator
    {
        return cbegin();
    }

    template <class K, class S, class... L>
    inline auto xcoordinate<K, S, L...>::end() const noexcept -> const_iterator
    {
        return cend();
    }

    template <class K, class S, class... L>
    inline auto xcoordinate<K, S, L...>::cbegin() const noexcept -> const_iterator
    {
        return m_coordinate.cbegin();
    }

    template <class K, class S, class... L>
    inline auto xcoordinate<K, S, L...>::cend() const noexcept -> const_iterator
    {
        return m_coordinate.cend();
    }
    
    template <class K, class S, class... L>
    inline bool xcoordinate<K, S, L...>::operator==(const self_type& rhs) const noexcept
    {
        return m_coordinate == rhs.m_coordinate;
    }

    template <class K, class S, class... L>
    inline bool xcoordinate<K, S, L...>::operator!=(const self_type& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    template <class K, class S, class... L>
    template <class LB1, class... LB>
    inline void xcoordinate<K, S, L...>::insert_impl(std::pair<K, xaxis<LB1, S>> axis, std::pair<K, xaxis<LB, S>>... axes)
    {
        m_coordinate.insert(value_type(std::move(axis.first), std::move(axis.second)));
        insert_impl(std::move(axes)...);
    }

    template <class K, class S, class... L>
    inline void xcoordinate<K, S, L...>::insert_impl()
    {
    }

    template <class K, class S, class... L>
    xcoordinate<K, S, L...> coordinate(const std::unordered_map<K, xtl::variant<xaxis<L, S>...>>& axes)
    {
        return xcoordinate<K, S, L...>(axes);
    }

    template <class K, class S, class... L>
    xcoordinate<K, S, L...> coordinate(std::unordered_map<K, xtl::variant<xaxis<L, S>...>>&& axes)
    {
        return xcoordinate<K, S, L...>(std::move(axes));
    }

    template <class K, class S, class... L>
    inline xcoordinate<K, S, L...> coordinate(std::pair<K, xaxis<L, S>>... axes)
    {
        return xcoordinate<K, S, L...>(std::move(axes)...);
    }

}

#endif

