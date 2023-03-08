"""
Check that operations with capabilities work properly.
"""

from __future__ import print_function

import lldb
from lldbsuite.test import lldbsmock
from lldbsuite.test import lldbsmockutil
from lldbsuite.test import lldbtest
from lldbsuite.test.decorators import skipIfRemote


class CapabilityOperationTestBase(lldbtest.TestBase):

    NO_DEBUG_INFO_TESTCASE = True

    mydir = lldbtest.TestBase.compute_mydir(__file__)

    def setUp(self):
        super(CapabilityOperationTestBase, self).setUp()
        self.server = lldbsmock.GDBServer(self.remote_config,
                                          trace=self.TraceOn())
        self.server.serve()

    def tearDown(self):
        super(CapabilityOperationTestBase, self).tearDown()
        self.server.close()

    def connect_to_mock(self):
        """Make a connection to the mock GDB server."""

        target = self.dbg.CreateTarget(None)
        self.assertTrue(target, lldbtest.VALID_TARGET)

        return lldbsmockutil.connect(self, target, self.server)

class MorelloCapabilityOperationTestCase(CapabilityOperationTestBase):

    def setUp(self):
        super(MorelloCapabilityOperationTestCase, self).setUp()

    def remote_config(self, remote):
        """Configure the remote target."""

        lldbsmock.setup_morello(remote)

        # Write {tag=1, address=0x1000, attributes={[Store Load],
        # range=[0x1000, 0x2000)}} to c0.
        remote.registers['c0'].write(0x01c0000000600010000000000000001000)

        # Write {tag=1, address=0x2000, attributes={[Store Load],
        # range=[0x1000, 0x4000)}} to c9.
        remote.registers['c9'].write(0x01c0000000400010000000000000002000)

        # Write {tag=0, address=0x1000, attributes={[Store Load],
        # range=[0x1000, 0x4000)}} to c23.
        remote.registers['c23'].write(0x00c0000000400010000000000000001000)

        # 0x0010: ELF header for a completely empty program.
        remote.memory.write(0x0010, [
            0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x02, 0x00, 0xb7, 0x00, 0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x38, 0x00,
            0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00])

        # 0x0400: Capability with address = 0x0000000080000010,
        #         range = [0x80000020-0x80004000).
        remote.memory.write(
            0x0400,
            [0x10, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
             0x20, 0x00, 0x00, 0x40, 0x00, 0xc0, 0x50, 0xdf],
            [0x01])
        # 0x0410: Capability with address = 0x0000000080000008,
        #         range = [0x80000020-0x80004000).
        remote.memory.write(
            0x0410,
            [0x08, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
             0x20, 0x00, 0x00, 0x40, 0x00, 0xc0, 0x50, 0xdf],
            [0x01])
        # 0x0420: Capability with address = 0x0000000080000018,
        #         range = [0x80000020-0x80004000).
        remote.memory.write(
            0x0420,
            [0x18, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
             0x20, 0x00, 0x00, 0x40, 0x00, 0xc0, 0x50, 0xdf],
            [0x00])
        # 0x0430: Capability with address = 0x0000000080000020,
        #         range = [0x80000020-0x80004000).
        remote.memory.write(
            0x0430,
            [0x20, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
             0x20, 0x00, 0x00, 0x40, 0x00, 0xc0, 0x50, 0xdf],
            [0x01])
        # 0x0440: Capability with address = 0x0000000080000028,
        #         range = [0x80000020-0x80004000).
        remote.memory.write(
            0x0440,
            [0x28, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
             0x20, 0x00, 0x00, 0x40, 0x00, 0xc0, 0x50, 0xdf],
            [0x01])
        # 0x0450: Capability with address = 0x0000000080000040,
        #         range = [0x80000080-0x80004000).
        remote.memory.write(
            0x0450,
            [0x40, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
             0x80, 0x00, 0x00, 0x40, 0x00, 0xc0, 0x50, 0xdf],
            [0x01])
        # 0x0460: Capability with address = 0x0000000080000100,
        #         range = [0x80000200-0x80004000).
        remote.memory.write(
            0x0460,
            [0x00, 0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
             0x00, 0x02, 0x00, 0x40, 0x00, 0xc0, 0x50, 0xdf],
            [0x00])
        # 0x0470: Capability with address = 0x0000000000000010,
        #         range = [0x10-0x20).
        remote.memory.write(
            0x0470,
            [0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
             0x10, 0x00, 0x20, 0x40, 0x00, 0xc0, 0x50, 0xdf],
            [0x01])

    @skipIfRemote
    def test_register_operation(self):
        """
        Check that low-level operations with capability registers work
        correctly.
        """

        self.connect_to_mock()

        # Test register reads.
        self.expect('register read x0',
                    substrs=["x0 = 0x0000000000001000"])
        self.expect('register read c0',
                    substrs=["c0 = 0x01c0000000600010000000000000001000"])
        self.expect('register read -f capability c0',
                    substrs=["c0 = 0x1C0000000600010000000000000001000"])

        self.expect('register read x9',
                    substrs=["x9 = 0x0000000000002000"])
        self.expect('register read c9',
                    substrs=["c9 = 0x01c0000000400010000000000000002000"])
        self.expect('register read -f capability c9',
                    substrs=["c9 = 0x1C0000000400010000000000000002000"])

        self.expect('register read x23',
                    substrs=["x23 = 0x0000000000001000"])
        self.expect('register read c23',
                    substrs=["c23 = 0x00c0000000400010000000000000001000"])
        self.expect('register read -f capability c23',
                    substrs=["c23 = 0xC0000000400010000000000000001000"])

        # Test register writes.
        # Write something with the tag 0.
        self.runCmd('register write c0 0x00c0000000500030000000000000001004')
        self.expect('register read x0',
                    substrs=["x0 = 0x0000000000001004"])
        self.expect('register read c0',
                    substrs=["c0 = 0x00c0000000500030000000000000001004"])
        self.expect('register read -f capability c0',
                    substrs=["c0 = 0xC0000000500030000000000000001004"])

        # Write something with the tag 1 in the same register.
        self.runCmd('register write c0 0x01c0000000400020000000000000001000')
        self.expect('register read x0',
                    substrs=["x0 = 0x0000000000001000"])
        self.expect('register read c0',
                    substrs=["c0 = 0x01c0000000400020000000000000001000"])
        self.expect('register read -f capability c0',
                    substrs=["c0 = 0x1C0000000400020000000000000001000"])

    # TODO: Test state registers

    @skipIfRemote
    def test_memory_operation(self):
        """
        Check that low-level operations with tagged memory work correctly.
        """

        process = self.connect_to_mock()

        error = lldb.SBError()

        # Test untagged read at 0x400.
        content = process.ReadMemory(0x400, 16, error)
        self.assertTrue(error.Success())
        self.assertEqual(len(content), 16)
        self.assertEqual(content,
                         '\x10\x00\x00\x80\x00\x00\x00\x00'
                         '\x20\x00\x00\x40\x00\xc0\x50\xdf')

        # Test read at the same address but this time as a tagged one.
        content = process.ReadMemory(0x400, 17, error,
                                     lldb.eMemoryContentCap128)
        self.assertTrue(error.Success())
        self.assertEqual(len(content), 17)
        self.assertEqual(content,
                         '\x01'
                         '\x10\x00\x00\x80\x00\x00\x00\x00'
                         '\x20\x00\x00\x40\x00\xc0\x50\xdf')

        # Test tagged read of more than one value.
        content = process.ReadMemory(0x460, 34, error,
                                     lldb.eMemoryContentCap128)
        self.assertTrue(error.Success())
        self.assertEqual(len(content), 34)
        self.assertEqual(content,
                         '\x00'
                         '\x00\x01\x00\x80\x00\x00\x00\x00'
                         '\x00\x02\x00\x40\x00\xc0\x50\xdf'
                         '\x01'
                         '\x10\x00\x00\x00\x00\x00\x00\x00'
                         '\x10\x00\x20\x40\x00\xc0\x50\xdf')

        # Test that software breakpoint opcodes are replaced with the original
        # data.
        breakpoint = process.GetTarget().BreakpointCreateByAddress(0x40e)
        self.assertTrue(breakpoint, lldbtest.VALID_BREAKPOINT)
        content = process.ReadMemory(0x400, 34, error,
                                     lldb.eMemoryContentCap128)
        self.assertTrue(error.Success())
        self.assertEqual(len(content), 34)
        self.assertEqual(content,
                         '\x01'
                         '\x10\x00\x00\x80\x00\x00\x00\x00'
                         '\x20\x00\x00\x40\x00\xc0\x50\xdf'
                         '\x01'
                         '\x08\x00\x00\x80\x00\x00\x00\x00'
                         '\x20\x00\x00\x40\x00\xc0\x50\xdf')

    @skipIfRemote
    def test_memory_read_command(self):
        """
        Check that the 'memory read -C' command works correctly.
        """

        self.connect_to_mock()

        # Test tagged read with data formatted as bytes with ascii (the
        # default format).
        self.expect(
            'memory read -C 0x400',
            substrs=["0x00000400: [01] 10 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P.",
                     "0x00000410: [01] 08 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P.",
                     "0x00000420: [00] 18 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P.",
                     "0x00000430: [01] 20 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  . ....... ..@..P.",
                     "0x00000440: [01] 28 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  .(....... ..@..P.",
                     "0x00000450: [01] 40 00 00 80 00 00 00 00 80 00 00 40 00 c0 50 df  .@..........@..P.",
                     "0x00000460: [00] 00 01 00 80 00 00 00 00 00 02 00 40 00 c0 50 df  ............@..P.",
                     "0x00000470: [01] 10 00 00 00 00 00 00 00 10 00 20 40 00 c0 50 df  ........... @..P."])

        # Test tagged read with data formatted as bytes.
        self.expect(
            'memory read -C -f bytes 0x400',
            substrs=["0x00000400: [01] 10 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df",
                     "0x00000410: [01] 08 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df",
                     "0x00000420: [00] 18 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df",
                     "0x00000430: [01] 20 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df",
                     "0x00000440: [01] 28 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df",
                     "0x00000450: [01] 40 00 00 80 00 00 00 00 80 00 00 40 00 c0 50 df",
                     "0x00000460: [00] 00 01 00 80 00 00 00 00 00 02 00 40 00 c0 50 df",
                     "0x00000470: [01] 10 00 00 00 00 00 00 00 10 00 20 40 00 c0 50 df"])

        # Test tagged read with data formatted as capabilities.
        self.expect(
            'memory read -C -f capability 0x400',
            substrs=["0x00000400: 0x1DF50C000400000200000000080000010",
                     "0x00000410: 0x1DF50C000400000200000000080000008",
                     "0x00000420: 0xDF50C000400000200000000080000018",
                     "0x00000430: 0x1DF50C000400000200000000080000020",
                     "0x00000440: 0x1DF50C000400000200000000080000028",
                     "0x00000450: 0x1DF50C000400000800000000080000040",
                     "0x00000460: 0xDF50C000400002000000000080000100",
                     "0x00000470: 0x1DF50C000402000100000000000000010",
                    ])

        # Test that repeated tagged reads work correctly.
        self.expect(
            'memory read -C -c 2 0x400',
            substrs=["0x00000400: [01] 10 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P.",
                     "0x00000410: [01] 08 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P."])
        self.expect(
            'memory read',
            substrs=["0x00000420: [00] 18 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P.",
                     "0x00000430: [01] 20 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  . ....... ..@..P."])
        self.expect(
            'memory read',
            substrs=["0x00000440: [01] 28 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  .(....... ..@..P.",
                     "0x00000450: [01] 40 00 00 80 00 00 00 00 80 00 00 40 00 c0 50 df  .@..........@..P."])

        # Test that repeated reads work correctly when the type is changed.
        self.expect(
            'memory read -C -c 2 0x400',
            substrs=["0x00000400: [01] 10 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P.",
                     "0x00000410: [01] 08 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P."])
        self.expect(
            'memory read -f bytes',
            substrs=["0x00000420: 18 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df",
                     "0x00000430: 20 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df"])
        self.expect(
            'memory read -C',
            substrs=["0x00000440: [01] 28 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  .(....... ..@..P.",
                     "0x00000450: [01] 40 00 00 80 00 00 00 00 80 00 00 40 00 c0 50 df  .@..........@..P."])

        # Test that the start address and total base byte size get
        # automatically aligned.
        self.expect(
            'memory read -C -c 1 0x408',
            substrs=["0x00000400: [01] 10 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P."])
        self.expect(
            'memory read -C 0x400 0x408',
            substrs=["0x00000400: [01] 10 00 00 80 00 00 00 00 20 00 00 40 00 c0 50 df  ......... ..@..P."])

        # Test that invalid formats are rejected when reading tagged memory.
        self.expect(
            'memory read -C -f hex 0x400', error=True,
            substrs=["error: display format \"hex\" is not supported when reading tagged memory"])

    @skipIfRemote
    def test_type_print(self):
        """
        Check operations with capability types.
        """

        process = self.connect_to_mock()

        # Add a dummy module to the target.
        module = lldb.SBModule(process, 0x10)
        self.assertTrue(module.IsValid())

        # Synthethize the 'int * __capability' type.
        int_type = module.GetBasicType(lldb.eBasicTypeInt)
        cap_ptr_type = int_type.GetPointerType(lldb.eAddressSpaceCapability)

        # Print capability at 0x470 using this type. The capability points at
        # the start of the module where the ELF header is (0x10).
        target = process.GetTarget()
        address = lldb.SBAddress(0x470, target)
        cap_ptr_value = target.CreateValueFromAddress("cap_ptr", address,
                                                      cap_ptr_type)
        self.assertEqual(cap_ptr_value.GetTypeName(), "int * __capability")
        self.assertEqual(cap_ptr_value.GetValue(),
                         "0x1DF50C000402000100000000000000010")
        cap_ptr_value.SetFormat(lldb.eFormatHex)
        self.assertEqual(cap_ptr_value.GetValue(),
                         "0x01df50c000402000100000000000000010")

        # Dereference the value.
        ptr_deref_value = cap_ptr_value.Dereference()
        ptr_deref_value.SetFormat(lldb.eFormatHex)
        self.assertEqual(ptr_deref_value.GetValue(), "0x464c457f")

        # Perform the same test for the 'int & __capability' type.
        cap_ref_type = int_type.GetReferenceType(lldb.eAddressSpaceCapability)
        cap_ref_value = target.CreateValueFromAddress("cap_ref", address,
                                                      cap_ref_type)
        self.assertEqual(cap_ref_value.GetTypeName(), "int & __capability")
        self.assertEqual(cap_ref_value.GetValue(),
                         "0x1DF50C000402000100000000000000010")
        cap_ref_value.SetFormat(lldb.eFormatHex)
        self.assertEqual(cap_ref_value.GetValue(),
                         "0x01df50c000402000100000000000000010")
        ref_deref_value = cap_ref_value.Dereference()
        ref_deref_value.SetFormat(lldb.eFormatHex)
        self.assertEqual(ref_deref_value.GetValue(), "0x464c457f")

        # Check the intcap_t type.
        intcap_type = module.GetBasicType(lldb.eBasicTypeSignedIntCap)
        intcap_value = target.CreateValueFromAddress("intcap", address,
                                                     intcap_type)
        self.assertEqual(intcap_value.GetTypeName(), "__intcap_t")
        self.assertEqual(intcap_value.GetValue(),
                         "0x1DF50C000402000100000000000000010")
        intcap_value.SetFormat(lldb.eFormatHex)
        self.assertEqual(intcap_value.GetValue(),
                         "0x01df50c000402000100000000000000010")

        # Check the uintcap_t type.
        uintcap_type = module.GetBasicType(lldb.eBasicTypeUnsignedIntCap)
        uintcap_value = target.CreateValueFromAddress("uintcap", address,
                                                      uintcap_type)
        self.assertEqual(uintcap_value.GetTypeName(), "__uintcap_t")
        self.assertEqual(uintcap_value.GetValue(),
                         "0x1DF50C000402000100000000000000010")
        uintcap_value.SetFormat(lldb.eFormatHex)
        self.assertEqual(uintcap_value.GetValue(),
                         "0x01df50c000402000100000000000000010")
