/*=============================================================================
    Spirit v1.6.1
    Copyright (c) 1998-2003 Joel de Guzman
    Copyright (c) 2002 Raghavendra Satish
    Copyright (c) 2002 Jeff Westfahl
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_LOOPS_HPP)
#define BOOST_SPIRIT_LOOPS_HPP

///////////////////////////////////////////////////////////////////////////////
#include "boost/spirit/core/parser.hpp"
#include "boost/spirit/core/composite/composite.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

    ///////////////////////////////////////////////////////////////////////////
    //
    //  fixed_loop class
    //
    //      This class takes care of the construct:
    //
    //          repeat_p (exact) [p]
    //
    //      where 'p' is a parser and 'exact' is the number of times to
    //      repeat. The parser iterates over the input exactly 'exact' times.
    //      The parse function fails if the parser does not match the input
    //      exactly 'exact' times.
    //
    //      This class is parametizable and can accept constant arguments
    //      (e.g. repeat_p (5) [p]) as well as references to variables (e.g.
    //      repeat_p (ref (n)) [p]).
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ParserT, typename ExactT>
    class fixed_loop
    : public unary<ParserT, parser <fixed_loop <ParserT, ExactT> > >
    {
    public:

        typedef fixed_loop<ParserT, ExactT>     self_t;
        typedef unary<ParserT, parser<self_t> >  base_t;

        fixed_loop (ParserT const & subject, ExactT const & exact)
        : base_t(subject), m_exact(exact) {}

        template <typename ScannerT>
        typename parser_result <self_t, ScannerT>::type
        parse (ScannerT const & scan) const
        {
            typename parser_result<self_t, ScannerT>::type hit(0);
            unsigned n = m_exact;

            for (unsigned i = 0; i < n; ++i)
            {
                typename parser_result<self_t, ScannerT>::type next
                    = this->subject().parse(scan);
                if (!next)
                    return scan.no_match();
                hit.concat(next);
            }

            return hit;
        }

        template <typename ScannerT>
        struct result
        {
            typedef typename match_result<ScannerT, nil_t>::type type;
        };

    private:

        ExactT m_exact;
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  finite_loop class
    //
    //      This class takes care of the construct:
    //
    //          repeat_p (min, max) [p]
    //
    //      where 'p' is a parser, 'min' and 'max' specifies the minimum and
    //      maximum iterations over 'p'. The parser iterates over the input
    //      at least 'min' times and at most 'max' times. The parse function
    //      fails if the parser does not match the input at least 'min' times
    //      and at most 'max' times.
    //
    //      This class is parametizable and can accept constant arguments
    //      (e.g. repeat_p (5, 10) [p]) as well as references to variables
    //      (e.g. repeat_p (ref (n1), ref (n2)) [p]).
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename ParserT, typename MinT, typename MaxT>
    class finite_loop
    : public unary<ParserT, parser<finite_loop<ParserT, MinT, MaxT> > >
    {
    public:

        typedef finite_loop <ParserT, MinT, MaxT> self_t;
        typedef unary<ParserT, parser<self_t> >   base_t;

        finite_loop (ParserT const & subject, MinT const & min, MaxT const & max)
        : base_t(subject), m_min(min), m_max(max) {}

        template <typename ScannerT>
        typename parser_result <self_t, ScannerT>::type
        parse(ScannerT const & scan) const
        {
            BOOST_SPIRIT_ASSERT(m_min <= m_max);
            typename parser_result<self_t, ScannerT>::type hit(0);

            unsigned n1 = m_min;
            unsigned n2 = m_max;

            for (unsigned i = 0; i < n2; ++i)
            {
                typename ScannerT::iterator_t save = scan.first;
                typename parser_result<self_t, ScannerT>::type next
                    = this->subject().parse(scan);

                if (!next)
                {
                    if (i >= n1)
                    {
                        scan.first = save;
                        break;
                    }
                    else
                    {
                        return scan.no_match();
                    }
                }

                hit.concat(next);
            }

            return hit;
        }

        template <typename ScannerT>
        struct result
        {
            typedef typename match_result<ScannerT, nil_t>::type type;
        };

    private:

        MinT    m_min;
        MaxT    m_max;
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  infinite_loop class
    //
    //      This class takes care of the construct:
    //
    //          repeat_p (min, more) [p]
    //
    //      where 'p' is a parser, 'min' is the minimum iteration over 'p'
    //      and more specifies that the iteration should proceed
    //      indefinitely. The parser iterates over the input at least 'min'
    //      times and continues indefinitely until 'p' fails or all of the
    //      input is parsed. The parse function fails if the parser does not
    //      match the input at least 'min' times.
    //
    //      This class is parametizable and can accept constant arguments
    //      (e.g. repeat_p (5, more) [p]) as well as references to variables
    //      (e.g. repeat_p (ref (n), more) [p]).
    //
    ///////////////////////////////////////////////////////////////////////////////

    struct more_t {};
    more_t const more = more_t ();

    template <typename ParserT, typename MinT>
    class infinite_loop
     : public unary<ParserT, parser<infinite_loop<ParserT, MinT> > >
    {
    public:

        typedef infinite_loop <ParserT, MinT>   self_t;
        typedef unary<ParserT, parser<self_t> > base_t;

        infinite_loop (
            ParserT const& subject,
            MinT const& min,
            more_t const&
        )
        : base_t(subject), m_min(min) {}

        template <typename ScannerT>
        typename parser_result <self_t, ScannerT>::type
        parse(ScannerT const & scan) const
        {
            typename parser_result<self_t, ScannerT>::type hit(0);
            unsigned n = m_min;

            for (unsigned i = 0; ; ++i)
            {
                typename ScannerT::iterator_t save = scan.first;
                typename parser_result<self_t, ScannerT>::type next
                    = this->subject().parse(scan);

                if (!next)
                {
                    if (i >= n)
                    {
                        scan.first = save;
                        break;
                    }
                    else
                    {
                        return scan.no_match();
                    }
                }

                hit.concat(next);
            }

            return hit;
        }

        template <typename ScannerT>
        struct result
        {
            typedef typename match_result<ScannerT, nil_t>::type type;
        };

        private:

        MinT m_min;
    };

    template <typename ExactT>
    struct fixed_loop_gen
    {
        fixed_loop_gen (ExactT const & exact)
        : m_exact (exact) {}

        template <typename ParserT>
        fixed_loop <ParserT, ExactT>
        operator[](parser <ParserT> const & subject) const
        {
            return fixed_loop <ParserT, ExactT> (subject.derived (), m_exact);
        }

        ExactT m_exact;
    };

    namespace impl {

    #if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

        template <typename ParserT, typename MinT, typename MaxT>
        struct loop_traits
        {
            typedef finite_loop<ParserT, MinT, MaxT> type;
        };

        template <typename ParserT, typename MinT>
        struct loop_traits<ParserT, MinT, more_t>
        {
            typedef infinite_loop<ParserT, MinT> type;
        };

    #else

        template <typename ParserT, typename MinT, typename MaxT>
        struct loop_traits
        {
            typedef typename mpl::if_<
                boost::is_same<MaxT, more_t>,
                infinite_loop<ParserT, MinT>,
                finite_loop<ParserT, MinT, MaxT>
            >::type type;
        };

    #endif

    } // namespace impl

    template <typename MinT, typename MaxT>
    struct nonfixed_loop_gen
    {
       nonfixed_loop_gen (MinT min, MaxT max)
        : m_min (min), m_max (max) {}

       template <typename ParserT>
       typename impl::loop_traits<ParserT, MinT, MaxT>::type
       operator[](parser <ParserT> const & subject) const
       {
           typedef typename impl::loop_traits<ParserT, MinT, MaxT>::type ret_t;
           return ret_t(
                subject.derived(),
                m_min,
                m_max);
       }

       MinT m_min;
       MaxT m_max;
    };

    template <typename ExactT>
    fixed_loop_gen <ExactT>
    repeat_p(ExactT const & exact)
    {
        return fixed_loop_gen <ExactT> (exact);
    }

    template <typename MinT, typename MaxT>
    nonfixed_loop_gen <MinT, MaxT>
    repeat_p(MinT const & min, MaxT const & max)
    {
        return nonfixed_loop_gen <MinT, MaxT> (min, max);
    }

}} // namespace boost::spirit

#endif // #if !defined(BOOST_SPIRIT_LOOPS_HPP)
