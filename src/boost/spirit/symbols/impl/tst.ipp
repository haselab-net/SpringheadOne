/*=============================================================================
    Spirit v1.6.1
    Copyright (c) 2001-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#ifndef BOOST_SPIRIT_TST_IPP
#define BOOST_SPIRIT_TST_IPP

///////////////////////////////////////////////////////////////////////////////
#include <memory> // for std::auto_ptr

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

    namespace impl
    {

///////////////////////////////////////////////////////////////////////////////
//
//  tst class
//
//      Ternary Search Tree implementation. The data structure is faster than
//      hashing for many typical search problems especially when the search
//      interface is iterator based. Searching for a string of length k in a
//      ternary search tree with n strings will require at most O(log n+k)
//      character comparisons. TSTs are many times faster than hash tables
//      for unsuccessful searches since mismatches are discovered earlier
//      after examining only a few characters. Hash tables always examine an
//      entire key when searching.
//
//      For details see http://www.cs.princeton.edu/~rs/strings/.
//
//      *** This is a low level class and is
//          not meant for public consumption ***
//
///////////////////////////////////////////////////////////////////////////////
    template <typename T, typename CharT>
    struct tst_node
    {
        tst_node(CharT value_)
        : value(value_)
        , left(0)
        , right(0)
        { middle.link = 0; }

        ~tst_node()
        {
            delete left;
            delete right;
            if (value)
                delete middle.link;
            else
                delete middle.data;
        }

        tst_node*
        clone() const
        {
            std::auto_ptr<tst_node> copy(new tst_node(value));

            if (left)
                copy->left = left->clone();
            if (right)
                copy->right = right->clone();

            if (value && middle.link)
            {
                copy->middle.link = middle.link->clone();
            }
            else
            {
                std::auto_ptr<T> mid_data(new T(*middle.data));
                copy->middle.data = mid_data.release();
            }

            return copy.release();
        }

        union center {

            tst_node*   link;
            T*          data;
        };

        CharT       value;
        tst_node*   left;
        center      middle;
        tst_node*   right;
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename CharT>
    class tst
    {
    public:

        struct search_info
        {
            T*          data;
            unsigned    length;
        };

        tst()
        : root(0) {}

        tst(tst const& other)
        : root(other.root ? other.root->clone() : 0) {}

        ~tst()
        { delete root; }

        tst&
        operator=(tst const& other)
        {
            if (this != &other)
            {
                node_t* new_root = other.root ? other.root->clone() : 0;
                delete root;
                root = new_root;
            }
            return *this;
        }

        template <typename IteratorT>
        T* add(IteratorT first, IteratorT const& last, T const& data)
        {
            if (first == last)
                return 0;

            node_t**  np = &root;
            CharT   ch = *first;

            for (;;)
            {
                if (*np == 0 || ch == 0)
                {
                    node_t* right = 0;
                    if (np != 0)
                        right = *np;
                    *np = new node_t(ch);
                    if (right)
                        (**np).right = right;
                }

                if (ch < (**np).value)
                {
                    np = &(**np).left;
                }
                else
                {
                    if (ch == (**np).value)
                    {
                        if (ch == 0)
                        {
                            if ((**np).middle.data == 0)
                            {
                                (**np).middle.data = new T(data);
                                return (**np).middle.data;
                            }
                            else
                            {
                                //  re-addition is disallowed
                                return 0;
                            }
                       }
                        ++first;
                        ch = (first == last) ? 0 : *first;
                        np = &(**np).middle.link;
                    }
                    else
                    {
                        np = &(**np).right;
                    }
                }
            }
        }

        template <typename ScannerT>
        search_info find(ScannerT const& scan) const
        {
            search_info result = { 0, 0 };
            if (scan.at_end()) {
                return result;
            }

            typedef typename ScannerT::iterator_t iterator_t;
            node_t*     np = root;
            CharT       ch = *scan;
            iterator_t  save = scan.first;
            iterator_t  latest = scan.first;
            unsigned    latest_len = 0;

            while (np)
            {
                if (ch < np->value)
                {
                    if (np->value == 0)
                    {
                        result.data = np->middle.data;
                        if (result.data)
                        {
                            latest = scan.first;
                            latest_len = result.length;
                        }
                    }
                    np = np->left;
                }
                else
                {
                    if (ch == np->value)
                    {
                        if (scan.at_end())
                        {
                            result.data = np->middle.data;
                            if (result.data)
                            {
                                latest = scan.first;
                                latest_len = result.length;
                            }
                            break;
                        }

                        ++scan;
                        ch = scan.at_end() ? 0 : *scan;
                        np = np->middle.link;
                        ++result.length;
                    }
                    else
                    {
                        if (np->value == 0)
                        {
                            result.data = np->middle.data;
                            if (result.data)
                            {
                                latest = scan.first;
                                latest_len = result.length;
                            }
                        }
                        np = np->right;
                     }
                }
            }

            if (result.data == 0)
            {
                scan.first = save;
            }
            else
            {
                scan.first = latest;
                result.length = latest_len;
            }
            return result;
        }

    private:

        typedef tst_node<T, CharT> node_t;
        node_t* root;
    };

///////////////////////////////////////////////////////////////////////////////
    } // namespace impl

}} // namespace boost::spirit

#endif
