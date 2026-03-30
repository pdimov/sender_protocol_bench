// Copyright 2026 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "proto_read.hpp"
#include "proto_write.hpp"
#include "simple_socket_source.hpp"
#include "simple_socket_sink.hpp"
#include "reader_task.hpp"
#include "writer_task.hpp"
#include <sendosio/sendosio.hpp>
#include <beman/execution/execution.hpp>
#include <boost/current_function.hpp>
#include <chrono>
#include <cstdio>
#include <thread>

namespace ex = beman::execution;

template<class Source, class Sink> void bench( std::vector<element> const& v )
{
    sendosio::io_context ioc;
    std::thread th;

    {
        auto [rsk, wsk] = sendosio::test::make_socket_pair( ioc );

        auto work_guard = boost::asio::make_work_guard( ioc );
        th = std::thread( [&]{ ioc.run(); } );

        std::printf( "-- %s:\n", BOOST_CURRENT_FUNCTION );

        auto t1 = std::chrono::steady_clock::now();

        auto rt = reader_task( Source( std::move(rsk) ) );
        auto wt = writer_task( Sink( std::move(wsk) ), v );

        auto pipe = ex::when_all( std::move(rt), std::move(wt) );

        ex::sync_wait( std::move( pipe ) );

        auto t2 = std::chrono::steady_clock::now();

        using namespace std::chrono_literals;

        std::printf( "-- %lld ms\n\n", static_cast<long long>( ( t2 - t1 ) / 1ms ) );
    }

    th.join();
}

int main()
{
    int const N = 100'000;

    std::vector<element> v( N );

    for( int j = 0; j < N; ++j )
    {
        v[ j ].index_ = j;
        v[ j ].key_ = "key" + std::to_string( j );
        v[ j ].value_.resize( 4, { j * 1.0f, j * 2.0f, j * 3.0f } );
    }

    bench<simple_socket_source, simple_socket_sink>( v );
}
