## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    module = bld.create_ns3_module('cosem', ['applications', 'internet'])
    module.source = [
        'model/cosem-al.cc',
        'model/cosem-al-server.cc',
        'model/cosem-ap-server.cc',
        'model/cosem-al-client.cc',
        'model/cosem-ap-client.cc',
        'model/udp-cosem-client.cc',
        'model/udp-cosem-server.cc',
        ]

   ## module_test = bld.create_ns3_module_test_library('cosem')
   ## module_test.source = [
   ##     'test/udp-client-server-test.cc',
   ##     ]

    headers = bld.new_task_gen(features=['ns3header'])
    headers.module = 'cosem'
    headers.source = [
        'model/cosem-al.h',
        'model/cosem-al-server.h',
        'model/cosem-ap-server.h',
        'model/cosem-al-client.h',
        'model/cosem-ap-client.h',
        'model/udp-cosem-client.h',
        'model/udp-cosem-server.h',
        ]

   # bld.ns3_python_bindings()
