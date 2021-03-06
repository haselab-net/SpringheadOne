/*=============================================================================
    Spirit v1.6.1
    Copyright (c) 2001-2003 Joel de Guzman
    Copyright (c) 2001-2003 Daniel Nuffer
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#ifndef BOOST_SPIRIT_BASIC_CHSET_HPP
#define BOOST_SPIRIT_BASIC_CHSET_HPP

///////////////////////////////////////////////////////////////////////////////
#include <bitset>
#include <boost/spirit/utility/impl/chset/range_run.hpp>

namespace boost { namespace spirit {

    ///////////////////////////////////////////////////////////////////////////
    //
    //  basic_chset: basic character set implementation using range_run
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    class basic_chset
    {
    public:
                            basic_chset();
                            basic_chset(basic_chset const& arg);

        bool                test(CharT v) const;
        void                set(CharT from, CharT to);
        void                set(CharT c);
        void                clear(CharT from, CharT to);
        void                clear(CharT c);
        void                clear();

        void                inverse();
        void                swap(basic_chset& x);

        basic_chset&        operator|=(basic_chset const& x);
        basic_chset&        operator&=(basic_chset const& x);
        basic_chset&        operator-=(basic_chset const& x);
        basic_chset&        operator^=(basic_chset const& x);

        private: utility::impl::range_run<CharT> rr;
    };

    #if (CHAR_BIT == 8)

    ///////////////////////////////////////////////////////////////////////////
    //
    //  basic_chset: specializations for 8 bit chars using std::bitset
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    class basic_chset_8bit {

    public:
                            basic_chset_8bit();
                            basic_chset_8bit(basic_chset_8bit const& arg);

        bool                test(CharT v) const;
        void                set(CharT from, CharT to);
        void                set(CharT c);
        void                clear(CharT from, CharT to);
        void                clear(CharT c);
        void                clear();

        void                inverse();
        void                swap(basic_chset_8bit& x);

        basic_chset_8bit&   operator|=(basic_chset_8bit const& x);
        basic_chset_8bit&   operator&=(basic_chset_8bit const& x);
        basic_chset_8bit&   operator-=(basic_chset_8bit const& x);
        basic_chset_8bit&   operator^=(basic_chset_8bit const& x);

        private: std::bitset<256> bset;
    };

    /////////////////////////////////
    template <>
    class basic_chset<char>
    : public basic_chset_8bit<char> {};

    /////////////////////////////////
    template <>
    class basic_chset<signed char>
    : public basic_chset_8bit<signed char> {};

    /////////////////////////////////
    template <>
    class basic_chset<unsigned char>
    : public basic_chset_8bit<unsigned char> {};

#endif

}} // namespace boost::spirit

#endif

#include <boost/spirit/utility/impl/chset/basic_chset.ipp>
