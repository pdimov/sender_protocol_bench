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

template<class Source, class Sink> void bench()
{
    std::thread th;

    {
        sendosio::io_context ioc;

        auto [rsk, wsk] = sendosio::test::make_socket_pair( ioc );

        auto work_guard = boost::asio::make_work_guard( ioc );
        th = std::thread( [&]{ ioc.run(); } );

        std::printf( "-- %s:\n", BOOST_CURRENT_FUNCTION );

        auto t1 = std::chrono::steady_clock::now();

        auto rt = reader_task( Source( std::move(rsk) ) );
        auto wt = writer_task( Sink( std::move(wsk) ) );

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
    bench<simple_socket_source, simple_socket_sink>();
}
