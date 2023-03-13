//
// impl/src.hpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_IMPL_SRC_HPP
#define ASIO_IMPL_SRC_HPP

#define ASIO_SOURCE

#include "ExternalLibraries/asio/detail/config.hpp"

#if defined(ASIO_HEADER_ONLY)
# error Do not compile Asio library source with ASIO_HEADER_ONLY defined
#endif

#include "ExternalLibraries/asio/impl/error.ipp"
#include "ExternalLibraries/asio/impl/error_code.ipp"
#include "ExternalLibraries/asio/impl/execution_context.ipp"
#include "ExternalLibraries/asio/impl/executor.ipp"
#include "ExternalLibraries/asio/impl/handler_alloc_hook.ipp"
#include "ExternalLibraries/asio/impl/io_context.ipp"
#include "ExternalLibraries/asio/impl/multiple_exceptions.ipp"
#include "ExternalLibraries/asio/impl/serial_port_base.ipp"
#include "ExternalLibraries/asio/impl/system_context.ipp"
#include "ExternalLibraries/asio/impl/thread_pool.ipp"
#include "ExternalLibraries/asio/detail/impl/buffer_sequence_adapter.ipp"
#include "ExternalLibraries/asio/detail/impl/descriptor_ops.ipp"
#include "ExternalLibraries/asio/detail/impl/dev_poll_reactor.ipp"
#include "ExternalLibraries/asio/detail/impl/epoll_reactor.ipp"
#include "ExternalLibraries/asio/detail/impl/eventfd_select_interrupter.ipp"
#include "ExternalLibraries/asio/detail/impl/handler_tracking.ipp"
#include "ExternalLibraries/asio/detail/impl/kqueue_reactor.ipp"
#include "ExternalLibraries/asio/detail/impl/null_event.ipp"
#include "ExternalLibraries/asio/detail/impl/pipe_select_interrupter.ipp"
#include "ExternalLibraries/asio/detail/impl/posix_event.ipp"
#include "ExternalLibraries/asio/detail/impl/posix_mutex.ipp"
#include "ExternalLibraries/asio/detail/impl/posix_thread.ipp"
#include "ExternalLibraries/asio/detail/impl/posix_tss_ptr.ipp"
#include "ExternalLibraries/asio/detail/impl/reactive_descriptor_service.ipp"
#include "ExternalLibraries/asio/detail/impl/reactive_serial_port_service.ipp"
#include "ExternalLibraries/asio/detail/impl/reactive_socket_service_base.ipp"
#include "ExternalLibraries/asio/detail/impl/resolver_service_base.ipp"
#include "ExternalLibraries/asio/detail/impl/scheduler.ipp"
#include "ExternalLibraries/asio/detail/impl/select_reactor.ipp"
#include "ExternalLibraries/asio/detail/impl/service_registry.ipp"
#include "ExternalLibraries/asio/detail/impl/signal_set_service.ipp"
#include "ExternalLibraries/asio/detail/impl/socket_ops.ipp"
#include "ExternalLibraries/asio/detail/impl/socket_select_interrupter.ipp"
#include "ExternalLibraries/asio/detail/impl/strand_executor_service.ipp"
#include "ExternalLibraries/asio/detail/impl/strand_service.ipp"
#include "ExternalLibraries/asio/detail/impl/throw_error.ipp"
#include "ExternalLibraries/asio/detail/impl/timer_queue_ptime.ipp"
#include "ExternalLibraries/asio/detail/impl/timer_queue_set.ipp"
#include "ExternalLibraries/asio/detail/impl/win_iocp_handle_service.ipp"
#include "ExternalLibraries/asio/detail/impl/win_iocp_io_context.ipp"
#include "ExternalLibraries/asio/detail/impl/win_iocp_serial_port_service.ipp"
#include "ExternalLibraries/asio/detail/impl/win_iocp_socket_service_base.ipp"
#include "ExternalLibraries/asio/detail/impl/win_event.ipp"
#include "ExternalLibraries/asio/detail/impl/win_mutex.ipp"
#include "ExternalLibraries/asio/detail/impl/win_object_handle_service.ipp"
#include "ExternalLibraries/asio/detail/impl/win_static_mutex.ipp"
#include "ExternalLibraries/asio/detail/impl/win_thread.ipp"
#include "ExternalLibraries/asio/detail/impl/win_tss_ptr.ipp"
#include "ExternalLibraries/asio/detail/impl/winrt_ssocket_service_base.ipp"
#include "ExternalLibraries/asio/detail/impl/winrt_timer_scheduler.ipp"
#include "ExternalLibraries/asio/detail/impl/winsock_init.ipp"
#include "ExternalLibraries/asio/execution/impl/bad_executor.ipp"
#include "ExternalLibraries/asio/execution/impl/receiver_invocation_error.ipp"
#include "ExternalLibraries/asio/generic/detail/impl/endpoint.ipp"
#include "ExternalLibraries/asio/ip/impl/address.ipp"
#include "ExternalLibraries/asio/ip/impl/address_v4.ipp"
#include "ExternalLibraries/asio/ip/impl/address_v6.ipp"
#include "ExternalLibraries/asio/ip/impl/host_name.ipp"
#include "ExternalLibraries/asio/ip/impl/network_v4.ipp"
#include "ExternalLibraries/asio/ip/impl/network_v6.ipp"
#include "ExternalLibraries/asio/ip/detail/impl/endpoint.ipp"
#include "ExternalLibraries/asio/local/detail/impl/endpoint.ipp"

#endif // ASIO_IMPL_SRC_HPP
