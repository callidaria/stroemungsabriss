	.file	"tmp_cmp.cpp"
# GNU C++17 (Ubuntu 11.4.0-1ubuntu1~22.04) version 11.4.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.4.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -O3 -std=c++17 -p -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
	.align 2
	.p2align 4
	.weak	_ZNKSt5ctypeIcE8do_widenEc
	.type	_ZNKSt5ctypeIcE8do_widenEc, @function
_ZNKSt5ctypeIcE8do_widenEc:
.LFB1558:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
1:	call	*mcount@GOTPCREL(%rip)
# /usr/include/c++/11/bits/locale_facets.h:1087:       do_widen(char __c) const
# /usr/include/c++/11/bits/locale_facets.h:1088:       { return __c; }
	popq	%rbp	#
	.cfi_def_cfa 7, 8
# /usr/include/c++/11/bits/locale_facets.h:1087:       do_widen(char __c) const
	movl	%esi, %eax	# tmp87, __c
# /usr/include/c++/11/bits/locale_facets.h:1088:       { return __c; }
	ret	
	.cfi_endproc
.LFE1558:
	.size	_ZNKSt5ctypeIcE8do_widenEc, .-_ZNKSt5ctypeIcE8do_widenEc
	.section	.text._ZNSt6vectorI16InitVariableInfoSaIS0_EED2Ev,"axG",@progbits,_ZNSt6vectorI16InitVariableInfoSaIS0_EED5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt6vectorI16InitVariableInfoSaIS0_EED2Ev
	.type	_ZNSt6vectorI16InitVariableInfoSaIS0_EED2Ev, @function
_ZNSt6vectorI16InitVariableInfoSaIS0_EED2Ev:
.LFB16547:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
1:	call	*mcount@GOTPCREL(%rip)
# /usr/include/c++/11/bits/stl_vector.h:678:       ~vector() _GLIBCXX_NOEXCEPT
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	(%rdi), %r8	# this_2(D)->D.55166._M_impl.D.54479._M_start, _1
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%r8, %r8	# _1
	je	.L3	#,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	16(%rdi), %rsi	# MEM[(struct _Vector_base *)this_2(D)]._M_impl.D.54479._M_end_of_storage, MEM[(struct _Vector_base *)this_2(D)]._M_impl.D.54479._M_end_of_storage
# /usr/include/c++/11/bits/stl_vector.h:683:       }
	popq	%rbp	#
	.cfi_remember_state
	.cfi_def_cfa 7, 8
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r8, %rdi	# _1,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%r8, %rsi	# _1, tmp87
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	jmp	_ZdlPvm@PLT	#
	.p2align 4,,10
	.p2align 3
.L3:
	.cfi_restore_state
# /usr/include/c++/11/bits/stl_vector.h:683:       }
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE16547:
	.size	_ZNSt6vectorI16InitVariableInfoSaIS0_EED2Ev, .-_ZNSt6vectorI16InitVariableInfoSaIS0_EED2Ev
	.weak	_ZNSt6vectorI16InitVariableInfoSaIS0_EED1Ev
	.set	_ZNSt6vectorI16InitVariableInfoSaIS0_EED1Ev,_ZNSt6vectorI16InitVariableInfoSaIS0_EED2Ev
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"iFrameResolutionPreset"
.LC1:
	.string	"vFrameResolutionWidth"
.LC2:
	.string	"vFrameResolutionHeight"
.LC3:
	.string	"bFrameFullscreen"
.LC4:
	.string	"iFrameTargetMonitor"
.LC5:
	.string	"iGraphicsTargetLibrary"
.LC6:
	.string	"iGraphicsPresetLevel"
.LC7:
	.string	"iGeneralLanguageWrittenText"
.LC8:
	.string	"iGeneralLanguageSpokenWord"
.LC9:
	.string	"iGeneralForcePeripheral"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC10:
	.string	"iGeneralPeripheralAxisDeadzone"
	.section	.rodata.str1.1
.LC11:
	.string	"iGeneralPeripheralAxisRange"
.LC12:
	.string	"fAudioMasterVolume"
.LC13:
	.string	"fAudioVoicesVolume"
.LC14:
	.string	"fAudioMusicVolume"
.LC15:
	.string	"fAudioEffectsVolume"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB18:
	.section	.text.startup,"ax",@progbits
.LHOTB18:
	.p2align 4
	.type	_Z41__static_initialization_and_destruction_0ii.constprop.0, @function
_Z41__static_initialization_and_destruction_0ii.constprop.0:
.LFB16556:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA16556
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%r13	#
	pushq	%r12	#
	subq	$272, %rsp	#,
	.cfi_offset 13, -24
	.cfi_offset 12, -32
1:	call	*mcount@GOTPCREL(%rip)
# tmp_cmp.cpp:185: }
# /usr/include/c++/11/iostream:74:   static ios_base::Init __ioinit;
	leaq	_ZStL8__ioinit(%rip), %r13	#, tmp91
	leaq	__dso_handle(%rip), %r12	#, tmp147
	movq	%r13, %rdi	# tmp91,
# tmp_cmp.cpp:185: }
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp150
	movq	%rax, -24(%rbp)	# tmp150, D.337301
	xorl	%eax, %eax	# tmp150
.LEHB0:
# /usr/include/c++/11/iostream:74:   static ios_base::Init __ioinit;
	call	_ZNSt8ios_base4InitC1Ev@PLT	#
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi	#,
	movq	%r12, %rdx	# tmp147,
	movq	%r13, %rsi	# tmp91,
	call	__cxa_atexit@PLT	#
# ./script/menu/../../ccb/fcn/init.h:20: const inline std::vector<InitVariableInfo> iKeys = {
	cmpb	$0, _ZGV5iKeys(%rip)	#, MEM[(char *)&_ZGV5iKeys]
	je	.L15	#,
.L6:
# ./script/menu/../../ccb/fcn/../gfx/../mat/camera2d.h:19: static Camera2D gCamera2D = Camera2D(1280.f,720.f);
	movq	-24(%rbp), %rax	# D.337301, tmp151
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp151
	jne	.L16	#,
	movss	.LC16(%rip), %xmm1	#,
	movss	.LC17(%rip), %xmm0	#,
# tmp_cmp.cpp:185: }
	addq	$272, %rsp	#,
# ./script/menu/../../ccb/fcn/../gfx/../mat/camera2d.h:19: static Camera2D gCamera2D = Camera2D(1280.f,720.f);
	leaq	_ZL9gCamera2D(%rip), %rdi	#, tmp145
# tmp_cmp.cpp:185: }
	popq	%r12	#
	popq	%r13	#
	popq	%rbp	#
	.cfi_remember_state
	.cfi_def_cfa 7, 8
# ./script/menu/../../ccb/fcn/../gfx/../mat/camera2d.h:19: static Camera2D gCamera2D = Camera2D(1280.f,720.f);
	jmp	_ZN8Camera2DC1Eff@PLT	#
.LEHE0:
.L15:
	.cfi_restore_state
# ./script/menu/../../ccb/fcn/init.h:20: const inline std::vector<InitVariableInfo> iKeys = {
	leaq	.LC0(%rip), %rax	#, tmp153
# /usr/include/c++/11/bits/stl_vector.h:98: 	: _M_start(), _M_finish(), _M_end_of_storage()
	pxor	%xmm0, %xmm0	# tmp115
# /usr/include/c++/11/ext/new_allocator.h:127: 	return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
	movl	$256, %edi	#,
# ./script/menu/../../ccb/fcn/init.h:20: const inline std::vector<InitVariableInfo> iKeys = {
	movb	$1, _ZGV5iKeys(%rip)	#, MEM[(char *)&_ZGV5iKeys]
	movq	%rax, -288(%rbp)	# tmp153, D.337256[0].name
	leaq	.LC1(%rip), %rax	#, tmp154
	movq	%rax, -272(%rbp)	# tmp154, D.337256[1].name
	leaq	.LC2(%rip), %rax	#, tmp155
	movq	%rax, -256(%rbp)	# tmp155, D.337256[2].name
	leaq	.LC3(%rip), %rax	#, tmp156
	movq	%rax, -240(%rbp)	# tmp156, D.337256[3].name
	leaq	.LC4(%rip), %rax	#, tmp157
	movq	%rax, -224(%rbp)	# tmp157, D.337256[4].name
	leaq	.LC5(%rip), %rax	#, tmp158
	movq	%rax, -208(%rbp)	# tmp158, D.337256[5].name
	leaq	.LC6(%rip), %rax	#, tmp159
	movq	%rax, -192(%rbp)	# tmp159, D.337256[6].name
	leaq	.LC7(%rip), %rax	#, tmp160
	movq	%rax, -176(%rbp)	# tmp160, D.337256[7].name
	leaq	.LC8(%rip), %rax	#, tmp161
	movq	%rax, -160(%rbp)	# tmp161, D.337256[8].name
	leaq	.LC9(%rip), %rax	#, tmp162
	movq	%rax, -144(%rbp)	# tmp162, D.337256[9].name
	leaq	.LC10(%rip), %rax	#, tmp163
	movq	%rax, -128(%rbp)	# tmp163, D.337256[10].name
	leaq	.LC11(%rip), %rax	#, tmp164
	movq	%rax, -112(%rbp)	# tmp164, D.337256[11].name
	leaq	.LC12(%rip), %rax	#, tmp165
	movq	%rax, -96(%rbp)	# tmp165, D.337256[12].name
	leaq	.LC13(%rip), %rax	#, tmp166
	movq	%rax, -80(%rbp)	# tmp166, D.337256[13].name
	leaq	.LC14(%rip), %rax	#, tmp167
	movq	%rax, -64(%rbp)	# tmp167, D.337256[14].name
	leaq	.LC15(%rip), %rax	#, tmp168
	movb	$1, -280(%rbp)	#, D.337256[0].restart_system_on_change
	movb	$0, -264(%rbp)	#, D.337256[1].restart_system_on_change
	movb	$0, -248(%rbp)	#, D.337256[2].restart_system_on_change
	movb	$1, -232(%rbp)	#, D.337256[3].restart_system_on_change
	movb	$1, -216(%rbp)	#, D.337256[4].restart_system_on_change
	movb	$1, -200(%rbp)	#, D.337256[5].restart_system_on_change
	movb	$0, -184(%rbp)	#, D.337256[6].restart_system_on_change
	movb	$0, -168(%rbp)	#, D.337256[7].restart_system_on_change
	movb	$0, -152(%rbp)	#, D.337256[8].restart_system_on_change
	movb	$0, -136(%rbp)	#, D.337256[9].restart_system_on_change
	movb	$0, -120(%rbp)	#, D.337256[10].restart_system_on_change
	movb	$0, -104(%rbp)	#, D.337256[11].restart_system_on_change
	movb	$0, -88(%rbp)	#, D.337256[12].restart_system_on_change
	movb	$0, -72(%rbp)	#, D.337256[13].restart_system_on_change
	movb	$0, -56(%rbp)	#, D.337256[14].restart_system_on_change
	movq	%rax, -48(%rbp)	# tmp168, D.337256[15].name
	movb	$0, -40(%rbp)	#, D.337256[15].restart_system_on_change
# /usr/include/c++/11/bits/stl_vector.h:98: 	: _M_start(), _M_finish(), _M_end_of_storage()
	movq	$0, 16+iKeys(%rip)	#, MEM[(struct _Vector_impl_data *)&iKeys]._M_end_of_storage
# /usr/include/c++/11/bits/stl_vector.h:98: 	: _M_start(), _M_finish(), _M_end_of_storage()
	movups	%xmm0, iKeys(%rip)	# tmp115, MEM <vector(2) long unsigned int> [(struct InitVariableInfo * *)&iKeys]
.LEHB1:
# /usr/include/c++/11/ext/new_allocator.h:127: 	return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
	call	_Znwm@PLT	#
.LEHE1:
	movq	-288(%rbp), %rcx	#, tmp124
# /usr/include/c++/11/bits/stl_vector.h:1583: 	  this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
	leaq	256(%rax), %rdx	#, _62
	leaq	8(%rax), %rdi	#, tmp132
# /usr/include/c++/11/bits/stl_vector.h:1582: 	    = this->_M_allocate(_S_check_init_len(__n, _M_get_Tp_allocator()));
	movq	%rax, iKeys(%rip)	# tmp117, iKeys.D.55166._M_impl.D.54479._M_start
# /usr/include/c++/11/bits/stl_vector.h:1583: 	  this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
	movq	%rdx, 16+iKeys(%rip)	# _62, iKeys.D.55166._M_impl.D.54479._M_end_of_storage
	andq	$-8, %rdi	#, tmp132
	leaq	-288(%rbp), %rsi	#, tmp120
	movq	%rcx, (%rax)	# tmp124,
	movq	-40(%rbp), %rcx	#, tmp131
	movq	%rcx, 248(%rax)	# tmp131,
	subq	%rdi, %rax	# tmp132, _72
	subq	%rax, %rsi	# _72, tmp122
	addl	$256, %eax	#, tmp123
	shrl	$3, %eax	#, tmp133
	movl	%eax, %ecx	# tmp133, tmp134
	rep movsq
# /usr/include/c++/11/bits/stl_vector.h:1584: 	  this->_M_impl._M_finish =
	movq	%rdx, 8+iKeys(%rip)	# _62, iKeys.D.55166._M_impl.D.54479._M_finish
# ./script/menu/../../ccb/fcn/init.h:20: const inline std::vector<InitVariableInfo> iKeys = {
	movq	%r12, %rdx	# tmp147,
	leaq	iKeys(%rip), %rsi	#, tmp118
	leaq	_ZNSt6vectorI16InitVariableInfoSaIS0_EED1Ev(%rip), %rdi	#, tmp138
	call	__cxa_atexit@PLT	#
	jmp	.L6	#
.L16:
# ./script/menu/../../ccb/fcn/../gfx/../mat/camera2d.h:19: static Camera2D gCamera2D = Camera2D(1280.f,720.f);
	call	__stack_chk_fail@PLT	#
.L11:
	endbr64	
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	%rax, %r12	# tmp149, tmp142
	jmp	.L7	#
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA16556:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE16556-.LLSDACSB16556
.LLSDACSB16556:
	.uleb128 .LEHB0-.LFB16556
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB16556
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L11-.LFB16556
	.uleb128 0
.LLSDACSE16556:
	.section	.text.startup
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC16556
	.type	_Z41__static_initialization_and_destruction_0ii.constprop.0.cold, @function
_Z41__static_initialization_and_destruction_0ii.constprop.0.cold:
.LFSB16556:
.L7:
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
	.cfi_offset 12, -32
	.cfi_offset 13, -24
	movq	iKeys(%rip), %rdi	# MEM[(struct _Vector_base *)&iKeys]._M_impl.D.54479._M_start, _52
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	16+iKeys(%rip), %rsi	# MEM[(struct _Vector_base *)&iKeys]._M_impl.D.54479._M_end_of_storage, _53
	subq	%rdi, %rsi	# _52, _53
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%rdi, %rdi	# _52
	je	.L8	#,
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L8:
	movq	%r12, %rdi	# tmp142,
.LEHB2:
	call	_Unwind_Resume@PLT	#
.LEHE2:
	.cfi_endproc
.LFE16556:
	.section	.gcc_except_table
.LLSDAC16556:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC16556-.LLSDACSBC16556
.LLSDACSBC16556:
	.uleb128 .LEHB2-.LCOLDB18
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSEC16556:
	.section	.text.unlikely
	.section	.text.startup
	.size	_Z41__static_initialization_and_destruction_0ii.constprop.0, .-_Z41__static_initialization_and_destruction_0ii.constprop.0
	.section	.text.unlikely
	.size	_Z41__static_initialization_and_destruction_0ii.constprop.0.cold, .-_Z41__static_initialization_and_destruction_0ii.constprop.0.cold
.LCOLDE18:
	.section	.text.startup
.LHOTE18:
	.section	.text._ZN19ListLanguageCommandD2Ev,"axG",@progbits,_ZN19ListLanguageCommandD5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZN19ListLanguageCommandD2Ev
	.type	_ZN19ListLanguageCommandD2Ev, @function
_ZN19ListLanguageCommandD2Ev:
.LFB13481:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%rbx	#
	subq	$8, %rsp	#,
	.cfi_offset 3, -24
1:	call	*mcount@GOTPCREL(%rip)
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movq	%rdi, %rbx	# tmp99, this
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	40(%rdi), %rdi	# MEM[(const struct basic_string *)this_1(D) + 40B]._M_dataplus._M_p, _8
# /usr/include/c++/11/bits/basic_string.h:211: 	return std::pointer_traits<const_pointer>::pointer_to(*_M_local_buf);
	leaq	56(%rbx), %rax	#, tmp92
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	%rax, %rdi	# tmp92, _8
	je	.L18	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	56(%rbx), %rax	# MEM[(struct basic_string *)this_1(D) + 40B].D.25367._M_allocated_capacity, tmp101
	leaq	1(%rax), %rsi	#, tmp93
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L18:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	8(%rbx), %rdi	# MEM[(const struct basic_string *)this_1(D) + 8B]._M_dataplus._M_p, _4
# /usr/include/c++/11/bits/basic_string.h:211: 	return std::pointer_traits<const_pointer>::pointer_to(*_M_local_buf);
	leaq	24(%rbx), %rax	#, tmp96
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	%rax, %rdi	# tmp96, _4
	je	.L17	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	24(%rbx), %rsi	# MEM[(struct basic_string *)this_1(D) + 8B].D.25367._M_allocated_capacity, MEM[(struct basic_string *)this_1(D) + 8B].D.25367._M_allocated_capacity
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movq	-8(%rbp), %rbx	#,
	leave	
	.cfi_remember_state
	.cfi_def_cfa 7, 8
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	addq	$1, %rsi	#, tmp97
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	jmp	_ZdlPvm@PLT	#
	.p2align 4,,10
	.p2align 3
.L17:
	.cfi_restore_state
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movq	-8(%rbp), %rbx	#,
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE13481:
	.size	_ZN19ListLanguageCommandD2Ev, .-_ZN19ListLanguageCommandD2Ev
	.weak	_ZN19ListLanguageCommandD1Ev
	.set	_ZN19ListLanguageCommandD1Ev,_ZN19ListLanguageCommandD2Ev
	.section	.text._ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED2Ev,"axG",@progbits,_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED2Ev
	.type	_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED2Ev, @function
_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED2Ev:
.LFB14326:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%r15	#
	pushq	%r14	#
	pushq	%r13	#
	pushq	%r12	#
	pushq	%rbx	#
	subq	$8, %rsp	#,
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
1:	call	*mcount@GOTPCREL(%rip)
# /usr/include/c++/11/bits/stl_vector.h:678:       ~vector() _GLIBCXX_NOEXCEPT
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	8(%rdi), %r14	# this_3(D)->D.279045._M_impl.D.278390._M_finish, _1
	movq	(%rdi), %r13	# this_3(D)->D.279045._M_impl.D.278390._M_start, __first
# /usr/include/c++/11/bits/stl_vector.h:678:       ~vector() _GLIBCXX_NOEXCEPT
	movq	%rdi, %r15	# tmp112, this
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r13, %r14	# __first, _1
	je	.L21	#,
	.p2align 4,,10
	.p2align 3
.L31:
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	32(%r13), %rbx	# MEM[(struct basic_string * *)__first_50 + 32B], _21
	movq	24(%r13), %r12	# MEM[(struct basic_string * *)__first_50 + 24B], __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r12, %rbx	# __first, _21
	je	.L22	#,
	.p2align 4,,10
	.p2align 3
.L26:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	(%r12), %rdi	# MEM[(char * *)__first_45], _28
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	16(%r12), %rax	#, tmp103
	cmpq	%rax, %rdi	# tmp103, _28
	je	.L23	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%r12), %rax	# MEM <size_type> [(union ._anon_79 *)__first_45 + 16B], tmp114
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$32, %r12	#, __first
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	leaq	1(%rax), %rsi	#, tmp104
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r12, %rbx	# __first, _21
	jne	.L26	#,
.L25:
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	24(%r13), %r12	# MEM[(struct basic_string * *)__first_50 + 24B], __first
.L22:
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%r12, %r12	# __first
	je	.L27	#,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	40(%r13), %rsi	# MEM[(struct basic_string * *)__first_50 + 40B], tmp106
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r12, %rdi	# __first,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%r12, %rsi	# __first, tmp106
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L27:
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	0(%r13), %rdi	# MEM[(short unsigned int * *)__first_50], _17
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%rdi, %rdi	# _17
	je	.L28	#,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	16(%r13), %rsi	# MEM[(short unsigned int * *)__first_50 + 16B], tmp108
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$48, %r13	#, __first
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%rdi, %rsi	# _17, tmp108
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r13, %r14	# __first, _1
	jne	.L31	#,
.L30:
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	(%r15), %r13	# MEM[(struct _Vector_base *)this_3(D)]._M_impl.D.278390._M_start, __first
.L21:
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%r13, %r13	# __first
	je	.L20	#,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	16(%r15), %rsi	# MEM[(struct _Vector_base *)this_3(D)]._M_impl.D.278390._M_end_of_storage, MEM[(struct _Vector_base *)this_3(D)]._M_impl.D.278390._M_end_of_storage
# /usr/include/c++/11/bits/stl_vector.h:683:       }
	addq	$8, %rsp	#,
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r13, %rdi	# __first,
# /usr/include/c++/11/bits/stl_vector.h:683:       }
	popq	%rbx	#
	popq	%r12	#
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%r13, %rsi	# __first, tmp110
# /usr/include/c++/11/bits/stl_vector.h:683:       }
	popq	%r13	#
	popq	%r14	#
	popq	%r15	#
	popq	%rbp	#
	.cfi_remember_state
	.cfi_def_cfa 7, 8
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	jmp	_ZdlPvm@PLT	#
	.p2align 4,,10
	.p2align 3
.L23:
	.cfi_restore_state
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$32, %r12	#, __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r12, %rbx	# __first, _21
	jne	.L26	#,
	jmp	.L25	#
	.p2align 4,,10
	.p2align 3
.L28:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$48, %r13	#, __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r13, %r14	# __first, _1
	jne	.L31	#,
	jmp	.L30	#
.L20:
# /usr/include/c++/11/bits/stl_vector.h:683:       }
	addq	$8, %rsp	#,
	popq	%rbx	#
	popq	%r12	#
	popq	%r13	#
	popq	%r14	#
	popq	%r15	#
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE14326:
	.size	_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED2Ev, .-_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED2Ev
	.weak	_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED1Ev
	.set	_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED1Ev,_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED2Ev
	.section	.rodata._ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_.str1.1,"aMS",@progbits,1
.LC19:
	.string	"vector::_M_realloc_insert"
	.section	.rodata._ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_.str1.8,"aMS",@progbits,1
	.align 8
.LC20:
	.string	"basic_string::_M_construct null not valid"
	.section	.text._ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_,"axG",@progbits,_ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_
	.type	_ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_, @function
_ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_:
.LFB15106:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA15106
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%r15	#
	pushq	%r14	#
	pushq	%r13	#
	pushq	%r12	#
	pushq	%rbx	#
	subq	$88, %rsp	#,
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
1:	call	*mcount@GOTPCREL(%rip)
# /usr/include/c++/11/bits/vector.tcc:426:       vector<_Tp, _Alloc>::
	movq	%rdi, -96(%rbp)	# this, %sfp
	movq	%rdx, %r14	# tmp305, __args#0
	movq	%rsi, %rbx	# tmp304, __position
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movabsq	$-3689348814741910323, %rdx	#, tmp207
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp314
	movq	%rax, -56(%rbp)	# tmp314, D.337465
	movq	%rdi, %rax	# this, tmp314
	movq	8(%rax), %r13	# MEM[(struct ListLanguageCommand * *)this_10(D) + 8B], _55
	movq	(%rax), %rcx	# MEM[(struct ListLanguageCommand * *)this_10(D)], _54
	movq	%r13, %rax	# _55, tmp204
	movq	%rcx, -80(%rbp)	# _54, %sfp
	subq	%rcx, %rax	# _54, tmp204
	sarq	$4, %rax	#, tmp205
	imulq	%rdx, %rax	# tmp207, tmp206
# /usr/include/c++/11/bits/stl_vector.h:1758: 	if (max_size() - size() < __n)
	movabsq	$115292150460684697, %rdx	#, tmp208
	cmpq	%rdx, %rax	# tmp208, tmp206
	je	.L102	#,
	testq	%rax, %rax	# tmp206
	movl	$1, %edx	#, tmp313
# /usr/include/c++/11/bits/stl_iterator.h:1271:     { return __lhs.base() - __rhs.base(); }
	movq	%rsi, %r12	# __position, _295
	cmovne	%rax, %rdx	# tmp206,, tmp210
	addq	%rdx, %rax	# tmp210, tmp211
	setc	%dl	#, _231
# /usr/include/c++/11/bits/stl_vector.h:1761: 	const size_type __len = size() + (std::max)(size(), __n);
	movq	%rax, -88(%rbp)	# __len, %sfp
# /usr/include/c++/11/bits/stl_iterator.h:1271:     { return __lhs.base() - __rhs.base(); }
	subq	-80(%rbp), %r12	# %sfp, _295
	movzbl	%dl, %edx	# _231, _231
# /usr/include/c++/11/bits/stl_vector.h:1762: 	return (__len < size() || __len > max_size()) ? max_size() : __len;
	testq	%rdx, %rdx	# _231
	jne	.L77	#,
# /usr/include/c++/11/bits/stl_vector.h:346: 	return __n != 0 ? _Tr::allocate(_M_impl, __n) : pointer();
	testq	%rax, %rax	# __len
	jne	.L41	#,
	movq	$0, -72(%rbp)	#, %sfp
.L75:
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movzbl	(%r14), %eax	# *__args#0_15(D).id, *__args#0_15(D).id
# /usr/include/c++/11/bits/vector.tcc:449: 	  _Alloc_traits::construct(this->_M_impl,
	addq	-72(%rbp), %r12	# %sfp, _2
# /usr/include/c++/11/bits/basic_string.h:927:       { return _M_string_length; }
	movq	16(%r14), %r15	# MEM[(const struct basic_string *)__args#0_15(D) + 8B]._M_string_length, _77
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movb	%al, (%r12)	# *__args#0_15(D).id, *_2.id
# /usr/include/c++/11/bits/basic_string.h:201: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	leaq	24(%r12), %rax	#, _75
	movq	%rax, -112(%rbp)	# _75, %sfp
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%rax, 8(%r12)	# _75, MEM[(struct _Alloc_hider *)_2 + 8B]._M_p
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	8(%r14), %rax	# MEM[(const struct basic_string *)__args#0_15(D) + 8B]._M_dataplus._M_p, _76
# /usr/include/c++/11/bits/basic_string.tcc:211: 	if (__gnu_cxx::__is_null_pointer(__beg) && __beg != __end)
	movq	%rax, %rcx	# _76, tmp315
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	%rax, -120(%rbp)	# _76, %sfp
# /usr/include/c++/11/bits/basic_string.tcc:211: 	if (__gnu_cxx::__is_null_pointer(__beg) && __beg != __end)
	addq	%r15, %rcx	# _77, tmp315
	je	.L42	#,
	testq	%rax, %rax	# _76
	je	.L103	#,
.L42:
# /usr/include/c++/11/bits/basic_string.tcc:215: 	size_type __dnew = static_cast<size_type>(std::distance(__beg, __end));
	movq	%r15, -64(%rbp)	# _77, MEM[(long unsigned int *)_30]
# /usr/include/c++/11/bits/basic_string.tcc:217: 	if (__dnew > size_type(_S_local_capacity))
	cmpq	$15, %r15	#, _77
	ja	.L104	#,
# /usr/include/c++/11/bits/basic_string.h:356: 	if (__n == 1)
	cmpq	$1, %r15	#, _77
	jne	.L45	#,
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movq	-120(%rbp), %rax	# %sfp, _76
	movzbl	(%rax), %eax	# MEM[(const char_type &)_76], _93
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	%al, 24(%r12)	# _93, MEM[(char_type &)_2 + 24]
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movq	-112(%rbp), %rax	# %sfp, prephitmp_57
.L46:
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	%r15, 16(%r12)	# _77, MEM[(struct basic_string *)_2 + 8B]._M_string_length
# /usr/include/c++/11/bits/basic_string.h:201: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	leaq	56(%r12), %rdi	#, _80
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%rax,%r15)	#, MEM[(char_type &)_96]
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	40(%r14), %rax	# MEM[(const struct basic_string *)__args#0_15(D) + 40B]._M_dataplus._M_p, _81
# /usr/include/c++/11/bits/basic_string.h:927:       { return _M_string_length; }
	movq	48(%r14), %r15	# MEM[(const struct basic_string *)__args#0_15(D) + 40B]._M_string_length, _82
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%rdi, 40(%r12)	# _80, MEM[(struct _Alloc_hider *)_2 + 40B]._M_p
# /usr/include/c++/11/bits/basic_string.tcc:211: 	if (__gnu_cxx::__is_null_pointer(__beg) && __beg != __end)
	movq	%rax, %rcx	# _81, tmp316
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	%rax, -120(%rbp)	# _81, %sfp
# /usr/include/c++/11/bits/basic_string.tcc:211: 	if (__gnu_cxx::__is_null_pointer(__beg) && __beg != __end)
	addq	%r15, %rcx	# _82, tmp316
	je	.L47	#,
	testq	%rax, %rax	# _81
	je	.L105	#,
.L47:
# /usr/include/c++/11/bits/basic_string.tcc:215: 	size_type __dnew = static_cast<size_type>(std::distance(__beg, __end));
	movq	%r15, -64(%rbp)	# _82, MEM[(long unsigned int *)_30]
# /usr/include/c++/11/bits/basic_string.tcc:217: 	if (__dnew > size_type(_S_local_capacity))
	cmpq	$15, %r15	#, _82
	ja	.L106	#,
# /usr/include/c++/11/bits/basic_string.h:356: 	if (__n == 1)
	cmpq	$1, %r15	#, _82
	jne	.L50	#,
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movq	-120(%rbp), %rax	# %sfp, _81
	movzbl	(%rax), %eax	# MEM[(const char_type &)_81], _102
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	%al, 56(%r12)	# _102, MEM[(char_type &)_2 + 56]
.L51:
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	%r15, 48(%r12)	# _82, MEM[(struct basic_string *)_2 + 40B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%rdi,%r15)	#, MEM[(char_type &)_105]
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movzwl	72(%r14), %eax	# *__args#0_15(D).line_number, *__args#0_15(D).line_number
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	movq	-72(%rbp), %r14	# %sfp, __cur
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movw	%ax, 72(%r12)	# *__args#0_15(D).line_number, *_2.line_number
	movq	-80(%rbp), %rax	# %sfp, _54
	leaq	24(%rax), %r12	#, ivtmp.443
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	cmpq	%rax, %rbx	# _54, __position
	jne	.L63	#,
	jmp	.L53	#
	.p2align 4,,10
	.p2align 3
.L57:
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, 8(%r14)	# _116, MEM[(char * *)__cur_134 + 8B]
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	(%r12), %rax	# MEM <size_type> [(union ._anon_79 *)_14], MEM <size_type> [(union ._anon_79 *)_14]
	movq	%rax, 24(%r14)	# MEM <size_type> [(union ._anon_79 *)_14], MEM <size_type> [(union ._anon_79 *)__cur_134 + 24B]
.L58:
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	-8(%r12), %rax	# MEM[(long unsigned int *)_14 + -8B], MEM[(long unsigned int *)_14 + -8B]
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%r12, -16(%r12)	# ivtmp.443, MEM[(char * *)_14 + -16B]
# /usr/include/c++/11/bits/basic_string.h:568: 	if (__str._M_is_local())
	leaq	32(%r12), %rdx	#, tmp256
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$0, -8(%r12)	#, MEM[(long unsigned int *)_14 + -8B]
	movq	%rax, 16(%r14)	# MEM[(long unsigned int *)_14 + -8B], MEM[(long unsigned int *)__cur_134 + 16B]
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	leaq	56(%r14), %rax	#, tmp255
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%r12)	#, MEM[(char_type &)_14]
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%rax, 40(%r14)	# tmp255, MEM[(char * *)__cur_134 + 40B]
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	16(%r12), %rax	# MEM[(char * *)_14 + 16B], _122
# /usr/include/c++/11/bits/basic_string.h:568: 	if (__str._M_is_local())
	cmpq	%rdx, %rax	# tmp256, _122
	je	.L107	#,
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, 40(%r14)	# _122, MEM[(char * *)__cur_134 + 40B]
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	32(%r12), %rax	# MEM <size_type> [(union ._anon_79 *)_14 + 32B], MEM <size_type> [(union ._anon_79 *)_14 + 32B]
	movq	%rax, 56(%r14)	# MEM <size_type> [(union ._anon_79 *)_14 + 32B], MEM <size_type> [(union ._anon_79 *)__cur_134 + 56B]
.L60:
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	24(%r12), %rax	# MEM[(long unsigned int *)_14 + 24B], MEM[(long unsigned int *)_14 + 24B]
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-16(%r12), %rdi	# MEM[(char * *)_14 + -16B], _110
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	%rax, 48(%r14)	# MEM[(long unsigned int *)_14 + 24B], MEM[(long unsigned int *)__cur_134 + 48B]
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movzwl	48(%r12), %eax	# MEM[(short unsigned int *)_14 + 48B], MEM[(short unsigned int *)_14 + 48B]
	movw	%ax, 72(%r14)	# MEM[(short unsigned int *)_14 + 48B], MEM[(short unsigned int *)__cur_134 + 72B]
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	%rdi, %r12	# _110, ivtmp.443
	je	.L61	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	(%r12), %rax	# MEM <size_type> [(union ._anon_79 *)_14], tmp341
	leaq	1(%rax), %rsi	#, tmp261
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L61:
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	leaq	80(%r12), %rax	#, ivtmp.443
	addq	$56, %r12	#, __first
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	addq	$80, %r14	#, __cur
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	cmpq	%r12, %rbx	# __first, __position
	je	.L53	#,
	movq	%rax, %r12	# ivtmp.443, ivtmp.443
.L63:
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movzbl	-24(%r12), %eax	# MEM[(unsigned char *)_14 + -24B], MEM[(unsigned char *)_14 + -24B]
	movb	%al, (%r14)	# MEM[(unsigned char *)_14 + -24B], MEM[(unsigned char *)__cur_134]
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	leaq	24(%r14), %rax	#, tmp251
	movq	%rax, 8(%r14)	# tmp251, MEM[(char * *)__cur_134 + 8B]
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-16(%r12), %rax	# MEM[(char * *)_14 + -16B], _116
# /usr/include/c++/11/bits/basic_string.h:568: 	if (__str._M_is_local())
	cmpq	%rax, %r12	# _116, ivtmp.443
	jne	.L57	#,
	movdqu	(%r12), %xmm1	# MEM <__int128 unsigned> [(char * {ref-all})_116], tmp339
	movups	%xmm1, 24(%r14)	# tmp339, MEM <__int128 unsigned> [(char * {ref-all})__cur_134 + 24B]
	jmp	.L58	#
	.p2align 4,,10
	.p2align 3
.L45:
# /usr/include/c++/11/bits/char_traits.h:431: 	if (__n == 0)
	testq	%r15, %r15	# _77
	jne	.L108	#,
	movq	-112(%rbp), %rax	# %sfp, prephitmp_57
	jmp	.L46	#
	.p2align 4,,10
	.p2align 3
.L107:
	movdqu	32(%r12), %xmm2	# MEM <__int128 unsigned> [(char * {ref-all})_14 + 32B], tmp340
	movups	%xmm2, 56(%r14)	# tmp340, MEM <__int128 unsigned> [(char * {ref-all})__cur_134 + 56B]
	jmp	.L60	#
	.p2align 4,,10
	.p2align 3
.L53:
# /usr/include/c++/11/bits/vector.tcc:464: 	      ++__new_finish;
	addq	$80, %r14	#, __new_finish
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	cmpq	%r13, %rbx	# _55, __position
	je	.L64	#,
	leaq	24(%rbx), %rax	#, ivtmp.430
	movq	%rbx, %rcx	# __position, __first
# /usr/include/c++/11/bits/stl_uninitialized.h:1030:       _ForwardIterator __cur = __result;
	movq	%r14, %rdx	# __new_finish, __cur
	.p2align 4,,10
	.p2align 3
.L70:
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movzbl	-24(%rax), %esi	# MEM[(unsigned char *)_168 + -24B], MEM[(unsigned char *)_168 + -24B]
	movb	%sil, (%rdx)	# MEM[(unsigned char *)_168 + -24B], MEM[(unsigned char *)__cur_301]
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	leaq	24(%rdx), %rsi	#, tmp266
	movq	%rsi, 8(%rdx)	# tmp266, MEM[(char * *)__cur_301 + 8B]
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-16(%rax), %rsi	# MEM[(char * *)_168 + -16B], _138
# /usr/include/c++/11/bits/basic_string.h:568: 	if (__str._M_is_local())
	cmpq	%rax, %rsi	# ivtmp.430, _138
	je	.L109	#,
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rsi, 8(%rdx)	# _138, MEM[(char * *)__cur_301 + 8B]
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	(%rax), %rsi	# MEM <size_type> [(union ._anon_79 *)_168], MEM <size_type> [(union ._anon_79 *)_168]
	movq	%rsi, 24(%rdx)	# MEM <size_type> [(union ._anon_79 *)_168], MEM <size_type> [(union ._anon_79 *)__cur_301 + 24B]
.L66:
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	-8(%rax), %rsi	# MEM[(long unsigned int *)_168 + -8B], MEM[(long unsigned int *)_168 + -8B]
# /usr/include/c++/11/bits/basic_string.h:568: 	if (__str._M_is_local())
	leaq	56(%rcx), %rdi	#, tmp271
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, -16(%rax)	# ivtmp.430, MEM[(char * *)_168 + -16B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$0, -8(%rax)	#, MEM[(long unsigned int *)_168 + -8B]
	movq	%rsi, 16(%rdx)	# MEM[(long unsigned int *)_168 + -8B], MEM[(long unsigned int *)__cur_301 + 16B]
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	leaq	56(%rdx), %rsi	#, tmp270
	movq	%rsi, 40(%rdx)	# tmp270, MEM[(char * *)__cur_301 + 40B]
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	16(%rax), %rsi	# MEM[(char * *)_168 + 16B], _144
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%rax)	#, MEM[(char_type &)_168]
# /usr/include/c++/11/bits/basic_string.h:568: 	if (__str._M_is_local())
	cmpq	%rdi, %rsi	# tmp271, _144
	je	.L110	#,
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rsi, 40(%rdx)	# _144, MEM[(char * *)__cur_301 + 40B]
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	32(%rax), %rsi	# MEM <size_type> [(union ._anon_79 *)_168 + 32B], MEM <size_type> [(union ._anon_79 *)_168 + 32B]
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	addq	$80, %rcx	#, __first
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	addq	$80, %rdx	#, __cur
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	addq	$80, %rax	#, ivtmp.430
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	%rsi, -24(%rdx)	# MEM <size_type> [(union ._anon_79 *)_168 + 32B], MEM <size_type> [(union ._anon_79 *)__cur_301 + 56B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	-56(%rax), %rsi	# MEM[(long unsigned int *)_168 + 24B], MEM[(long unsigned int *)_168 + 24B]
	movq	%rsi, -32(%rdx)	# MEM[(long unsigned int *)_168 + 24B], MEM[(long unsigned int *)__cur_301 + 48B]
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movzwl	-32(%rax), %esi	# MEM[(short unsigned int *)_168 + 48B], MEM[(short unsigned int *)_168 + 48B]
	movw	%si, -8(%rdx)	# MEM[(short unsigned int *)_168 + 48B], MEM[(short unsigned int *)__cur_301 + 72B]
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	cmpq	%r13, %rcx	# _55, __first
	jne	.L70	#,
.L69:
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	movabsq	$922337203685477581, %rdx	#, tmp283
	subq	%rbx, %r13	# __position, tmp278
	leaq	-80(%r13), %rax	#, tmp280
	shrq	$4, %rax	#, tmp281
	imulq	%rdx, %rax	# tmp283, tmp282
	movabsq	$1152921504606846975, %rdx	#, tmp285
	andq	%rdx, %rax	# tmp285, tmp284
	leaq	5(%rax,%rax,4), %rax	#, tmp289
	salq	$4, %rax	#, tmp290
	addq	%rax, %r14	# tmp290, __new_finish
.L64:
	movq	-72(%rbp), %xmm0	# %sfp, _229
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	movq	-80(%rbp), %rax	# %sfp, _54
	movq	%r14, %xmm5	# __new_finish, __new_finish
	punpcklqdq	%xmm5, %xmm0	# __new_finish, _229
	testq	%rax, %rax	# _54
	je	.L71	#,
# /usr/include/c++/11/bits/vector.tcc:501: 		    this->_M_impl._M_end_of_storage - __old_start);
	movq	-96(%rbp), %rbx	# %sfp, this
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%rax, %rdi	# _54,
	movaps	%xmm0, -112(%rbp)	# _229, %sfp
# /usr/include/c++/11/bits/vector.tcc:501: 		    this->_M_impl._M_end_of_storage - __old_start);
	movq	16(%rbx), %rbx	# this_10(D)->D.296089._M_impl.D.295434._M_end_of_storage, this_10(D)->D.296089._M_impl.D.295434._M_end_of_storage
	movq	%rbx, %rsi	# this_10(D)->D.296089._M_impl.D.295434._M_end_of_storage, this_10(D)->D.296089._M_impl.D.295434._M_end_of_storage
	movq	%rbx, -80(%rbp)	# this_10(D)->D.296089._M_impl.D.295434._M_end_of_storage, %sfp
	subq	%rax, %rsi	# _54, this_10(D)->D.296089._M_impl.D.295434._M_end_of_storage
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
	movdqa	-112(%rbp), %xmm0	# %sfp, _229
.L71:
# /usr/include/c++/11/bits/vector.tcc:502:       this->_M_impl._M_start = __new_start;
	movq	-96(%rbp), %rbx	# %sfp, this
# /usr/include/c++/11/bits/vector.tcc:504:       this->_M_impl._M_end_of_storage = __new_start + __len;
	movq	-72(%rbp), %rax	# %sfp, iftmp.42_18
	addq	-88(%rbp), %rax	# %sfp, iftmp.42_18
	movq	%rax, 16(%rbx)	# tmp294, this_10(D)->D.296089._M_impl.D.295434._M_end_of_storage
# /usr/include/c++/11/bits/vector.tcc:502:       this->_M_impl._M_start = __new_start;
	movups	%xmm0, (%rbx)	# _229, MEM <vector(2) long unsigned int> [(struct ListLanguageCommand * *)this_10(D)]
# /usr/include/c++/11/bits/vector.tcc:505:     }
	movq	-56(%rbp), %rax	# D.337465, tmp317
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp317
	jne	.L111	#,
	addq	$88, %rsp	#,
	popq	%rbx	#
	popq	%r12	#
	popq	%r13	#
	popq	%r14	#
	popq	%r15	#
	popq	%rbp	#
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret	
	.p2align 4,,10
	.p2align 3
.L110:
	.cfi_restore_state
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	24(%rax), %rsi	# MEM[(long unsigned int *)_168 + 24B], MEM[(long unsigned int *)_168 + 24B]
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	addq	$80, %rcx	#, __first
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	addq	$80, %rdx	#, __cur
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	addq	$80, %rax	#, ivtmp.430
	movdqu	-48(%rax), %xmm4	# MEM <__int128 unsigned> [(char * {ref-all})_168 + 32B], tmp343
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	%rsi, -32(%rdx)	# MEM[(long unsigned int *)_168 + 24B], MEM[(long unsigned int *)__cur_301 + 48B]
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movzwl	-32(%rax), %esi	# MEM[(short unsigned int *)_168 + 48B], MEM[(short unsigned int *)_168 + 48B]
	movups	%xmm4, -24(%rdx)	# tmp343, MEM <__int128 unsigned> [(char * {ref-all})__cur_301 + 56B]
	movw	%si, -8(%rdx)	# MEM[(short unsigned int *)_168 + 48B], MEM[(short unsigned int *)__cur_301 + 72B]
# /usr/include/c++/11/bits/stl_uninitialized.h:1031:       for (; __first != __last; ++__first, (void)++__cur)
	cmpq	%rcx, %r13	# __first, _55
	jne	.L70	#,
	jmp	.L69	#
	.p2align 4,,10
	.p2align 3
.L109:
	movdqu	(%rax), %xmm3	# MEM <__int128 unsigned> [(char * {ref-all})_168], tmp342
	movups	%xmm3, 24(%rdx)	# tmp342, MEM <__int128 unsigned> [(char * {ref-all})__cur_301 + 24B]
	jmp	.L66	#
	.p2align 4,,10
	.p2align 3
.L77:
	movabsq	$9223372036854775760, %rax	#, __len
	movq	%rax, -88(%rbp)	# __len, %sfp
	movq	%rax, %rdi	# __len,
.L40:
.LEHB3:
# /usr/include/c++/11/ext/new_allocator.h:127: 	return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
	call	_Znwm@PLT	#
.LEHE3:
	movq	%rax, -72(%rbp)	# tmp311, %sfp
	jmp	.L75	#
	.p2align 4,,10
	.p2align 3
.L50:
# /usr/include/c++/11/bits/char_traits.h:431: 	if (__n == 0)
	testq	%r15, %r15	# _82
	je	.L51	#,
	jmp	.L49	#
	.p2align 4,,10
	.p2align 3
.L106:
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	leaq	-64(%rbp), %rsi	#, tmp235
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	leaq	40(%r12), %rdi	#, tmp236
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	xorl	%edx, %edx	#
.LEHB4:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT	#
.LEHE4:
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, 40(%r12)	# _80, MEM[(struct basic_string *)_2 + 40B]._M_dataplus._M_p
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	movq	%rax, %rdi	# tmp307, _80
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	-64(%rbp), %rax	# MEM[(long unsigned int *)_30], MEM[(long unsigned int *)_30]
	movq	%rax, 56(%r12)	# MEM[(long unsigned int *)_30], MEM[(struct basic_string *)_2 + 40B].D.25367._M_allocated_capacity
.L49:
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	-120(%rbp), %rsi	# %sfp,
	movq	%r15, %rdx	# _82,
	call	memcpy@PLT	#
# /usr/include/c++/11/bits/basic_string.tcc:232: 	_M_set_length(__dnew);
	movq	-64(%rbp), %r15	# MEM[(long unsigned int *)_30], _82
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	40(%r12), %rdi	# MEM[(const struct basic_string *)_2 + 40B]._M_dataplus._M_p, _80
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	jmp	.L51	#
	.p2align 4,,10
	.p2align 3
.L104:
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	leaq	-64(%rbp), %rsi	#, tmp220
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	leaq	8(%r12), %rdi	#, tmp221
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	xorl	%edx, %edx	#
.LEHB5:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT	#
.LEHE5:
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, 8(%r12)	# _90, MEM[(struct basic_string *)_2 + 8B]._M_dataplus._M_p
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	movq	%rax, %rdi	# tmp306, _90
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	-64(%rbp), %rax	# MEM[(long unsigned int *)_30], MEM[(long unsigned int *)_30]
	movq	%rax, 24(%r12)	# MEM[(long unsigned int *)_30], MEM[(struct basic_string *)_2 + 8B].D.25367._M_allocated_capacity
.L44:
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	-120(%rbp), %rsi	# %sfp,
	movq	%r15, %rdx	# _77,
	call	memcpy@PLT	#
# /usr/include/c++/11/bits/basic_string.tcc:232: 	_M_set_length(__dnew);
	movq	-64(%rbp), %r15	# MEM[(long unsigned int *)_30], _77
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	8(%r12), %rax	# MEM[(const struct basic_string *)_2 + 8B]._M_dataplus._M_p, prephitmp_57
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	jmp	.L46	#
.L41:
	movq	%rax, %rcx	# __len, __len
# /usr/include/c++/11/ext/new_allocator.h:127: 	return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
	movabsq	$115292150460684697, %rax	#, tmp312
	cmpq	%rax, %rcx	# tmp312, __len
	cmovbe	%rcx, %rax	# __len,, tmp296
	leaq	(%rax,%rax,4), %rax	#, tmp299
	salq	$4, %rax	#, tmp299
	movq	%rax, -88(%rbp)	# tmp299, %sfp
	movq	%rax, %rdi	# tmp299,
	jmp	.L40	#
.L102:
# /usr/include/c++/11/bits/stl_vector.h:1759: 	  __throw_length_error(__N(__s));
	leaq	.LC19(%rip), %rdi	#, tmp209
.LEHB6:
	call	_ZSt20__throw_length_errorPKc@PLT	#
.LEHE6:
.L103:
# /usr/include/c++/11/bits/basic_string.tcc:212: 	  std::__throw_logic_error(__N("basic_string::"
	leaq	.LC20(%rip), %rdi	#, tmp219
.LEHB7:
	call	_ZSt19__throw_logic_errorPKc@PLT	#
.LEHE7:
.L105:
	leaq	.LC20(%rip), %rdi	#, tmp234
.LEHB8:
	call	_ZSt19__throw_logic_errorPKc@PLT	#
.LEHE8:
.L108:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-112(%rbp), %rdi	# %sfp, _90
	jmp	.L44	#
.L111:
# /usr/include/c++/11/bits/vector.tcc:505:     }
	call	__stack_chk_fail@PLT	#
.L81:
	endbr64	
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	%rax, %rbx	# tmp308, tmp247
	jmp	.L54	#
.L79:
	endbr64	
# /usr/include/c++/11/bits/vector.tcc:485:       __catch(...)
	movq	%rax, %rdi	# tmp309, tmp246
	jmp	.L56	#
.L54:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	8(%r12), %rdi	# MEM[(const struct basic_string *)_2 + 8B]._M_dataplus._M_p, _85
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	%rdi, -112(%rbp)	# _85, %sfp
	je	.L55	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	24(%r12), %rax	# MEM[(struct basic_string *)_2 + 8B].D.25367._M_allocated_capacity, tmp338
	leaq	1(%rax), %rsi	#, tmp244
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L55:
	movq	%rbx, %rdi	# tmp247, tmp246
.L56:
# /usr/include/c++/11/bits/vector.tcc:485:       __catch(...)
	call	__cxa_begin_catch@PLT	#
# /usr/include/c++/11/bits/vector.tcc:487: 	  if (!__new_finish)
	cmpq	$0, -72(%rbp)	#, %sfp
	je	.L112	#,
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	-88(%rbp), %rsi	# %sfp,
	movq	-72(%rbp), %rdi	# %sfp,
	call	_ZdlPvm@PLT	#
.L74:
.LEHB9:
# /usr/include/c++/11/bits/vector.tcc:493: 	  __throw_exception_again;
	call	__cxa_rethrow@PLT	#
.LEHE9:
.L112:
# /usr/include/c++/11/ext/new_allocator.h:168: 	{ __p->~_Up(); }
	movq	%r12, %rdi	# _2,
	call	_ZN19ListLanguageCommandD1Ev	#
	jmp	.L74	#
.L80:
	endbr64	
# /usr/include/c++/11/bits/vector.tcc:485:       __catch(...)
	movq	%rax, %r12	# tmp310, tmp295
.L73:
	call	__cxa_end_catch@PLT	#
	movq	%r12, %rdi	# tmp295,
.LEHB10:
	call	_Unwind_Resume@PLT	#
.LEHE10:
	.cfi_endproc
.LFE15106:
	.section	.gcc_except_table
	.align 4
.LLSDA15106:
	.byte	0xff
	.byte	0x9b
	.uleb128 .LLSDATT15106-.LLSDATTD15106
.LLSDATTD15106:
	.byte	0x1
	.uleb128 .LLSDACSE15106-.LLSDACSB15106
.LLSDACSB15106:
	.uleb128 .LEHB3-.LFB15106
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB4-.LFB15106
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L81-.LFB15106
	.uleb128 0x3
	.uleb128 .LEHB5-.LFB15106
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L79-.LFB15106
	.uleb128 0x1
	.uleb128 .LEHB6-.LFB15106
	.uleb128 .LEHE6-.LEHB6
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB7-.LFB15106
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L79-.LFB15106
	.uleb128 0x1
	.uleb128 .LEHB8-.LFB15106
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L81-.LFB15106
	.uleb128 0x3
	.uleb128 .LEHB9-.LFB15106
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L80-.LFB15106
	.uleb128 0
	.uleb128 .LEHB10-.LFB15106
	.uleb128 .LEHE10-.LEHB10
	.uleb128 0
	.uleb128 0
.LLSDACSE15106:
	.byte	0x1
	.byte	0
	.byte	0
	.byte	0x7d
	.align 4
	.long	0

.LLSDATT15106:
	.section	.text._ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_,"axG",@progbits,_ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_,comdat
	.size	_ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_, .-_ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_
	.section	.rodata.str1.1
.LC21:
	.string	"basic_string::substr"
	.section	.rodata.str1.8
	.align 8
.LC22:
	.string	"%s: __pos (which is %zu) > this->size() (which is %zu)"
	.align 8
.LC23:
	.string	"%s: \033[31;1mvariable linking error.\033[0m referenced as \"\033[34;1m%s\033[0m\"\n"
	.section	.text.unlikely
	.align 2
.LCOLDB25:
	.text
.LHOTB25:
	.align 2
	.p2align 4
	.globl	_ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE
	.type	_ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE, @function
_ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE:
.LFB13443:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA13443
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%r15	#
	pushq	%r14	#
	pushq	%r13	#
	pushq	%r12	#
	pushq	%rbx	#
	subq	$1528, %rsp	#,
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
1:	call	*mcount@GOTPCREL(%rip)
# tmp_cmp.cpp:100: {
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$26478, %ebx	#,
	movl	$25972, %r9d	#,
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	xorl	%edx, %edx	#
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$25966, %r10d	#,
	movl	$29556, %r11d	#,
	movl	$28261, %r14d	#,
	movl	$29806, %r15d	#,
# tmp_cmp.cpp:100: {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp978
	movq	%rax, -56(%rbp)	# tmp978, D.338014
	xorl	%eax, %eax	# tmp978
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movw	%bx, -876(%rbp)	#, MEM <char[1:7]> [(void *)&mlcmd + 144B]
# tmp_cmp.cpp:100: {
	movq	%rdi, %r12	# tmp952, path
	movq	%rsi, %r13	# tmp953, rClusters
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	-1008(%rbp), %rax	#, tmp473
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movw	%r9w, -1004(%rbp)	#, MEM <char[1:7]> [(void *)&mlcmd + 16B]
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	leaq	-736(%rbp), %rbx	#, _227
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	movq	%rax, -1024(%rbp)	# tmp473, MEM[(struct _Alloc_hider *)&mlcmd]._M_p
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	movq	%rbx, %rdi	# _227,
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	-976(%rbp), %rax	#, tmp477
	movq	%rax, -992(%rbp)	# tmp477, MEM[(struct _Alloc_hider *)&mlcmd + 32B]._M_p
	leaq	-944(%rbp), %rax	#, tmp481
	movq	%rax, -960(%rbp)	# tmp481, MEM[(struct _Alloc_hider *)&mlcmd + 64B]._M_p
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movabsq	$7305517485564061028, %rax	#, tmp1014
	movq	%rax, -944(%rbp)	# tmp1014, MEM <char[1:8]> [(void *)&mlcmd + 80B]
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	-912(%rbp), %rax	#, tmp486
	movq	%rax, -928(%rbp)	# tmp486, MEM[(struct _Alloc_hider *)&mlcmd + 96B]._M_p
	leaq	-880(%rbp), %rax	#, tmp490
	movq	%rax, -896(%rbp)	# tmp490, MEM[(struct _Alloc_hider *)&mlcmd + 128B]._M_p
	leaq	-848(%rbp), %rax	#, tmp494
	movq	%rax, -864(%rbp)	# tmp494, MEM[(struct _Alloc_hider *)&mlcmd + 160B]._M_p
	leaq	-816(%rbp), %rax	#, tmp498
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1937075299, -1008(%rbp)	#, MEM <char[1:7]> [(void *)&mlcmd + 16B]
	movb	$114, -1002(%rbp)	#, MEM <char[1:7]> [(void *)&mlcmd + 16B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$7, -1016(%rbp)	#, MEM[(struct basic_string *)&mlcmd]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -1001(%rbp)	#, MEM[(char_type &)&mlcmd + 23]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1768318308, -976(%rbp)	#, MEM <char[1:6]> [(void *)&mlcmd + 48B]
	movw	%r10w, -972(%rbp)	#, MEM <char[1:6]> [(void *)&mlcmd + 48B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$6, -984(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 32B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -970(%rbp)	#, MEM[(char_type &)&mlcmd + 54]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$8, -952(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 64B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -936(%rbp)	#, MEM[(char_type &)&mlcmd + 88]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1634692198, -912(%rbp)	#, MEM <char[1:6]> [(void *)&mlcmd + 112B]
	movw	%r11w, -908(%rbp)	#, MEM <char[1:6]> [(void *)&mlcmd + 112B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$6, -920(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 96B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -906(%rbp)	#, MEM[(char_type &)&mlcmd + 118]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1769108595, -880(%rbp)	#, MEM <char[1:7]> [(void *)&mlcmd + 144B]
	movb	$115, -874(%rbp)	#, MEM <char[1:7]> [(void *)&mlcmd + 144B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$7, -888(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 128B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -873(%rbp)	#, MEM[(char_type &)&mlcmd + 151]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1835492723, -848(%rbp)	#, MEM <char[1:7]> [(void *)&mlcmd + 176B]
	movw	%r14w, -844(%rbp)	#, MEM <char[1:7]> [(void *)&mlcmd + 176B]
	movb	$116, -842(%rbp)	#, MEM <char[1:7]> [(void *)&mlcmd + 176B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$7, -856(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 160B]._M_string_length
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	movq	%rax, -832(%rbp)	# tmp498, MEM[(struct _Alloc_hider *)&mlcmd + 192B]._M_p
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movabsq	$8028075806769966947, %rax	#, tmp1015
	movq	%rax, -816(%rbp)	# tmp1015, MEM <char[1:9]> [(void *)&mlcmd + 208B]
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	-784(%rbp), %rax	#, tmp503
	movq	%rax, -800(%rbp)	# tmp503, MEM[(struct _Alloc_hider *)&mlcmd + 224B]._M_p
	leaq	-752(%rbp), %rax	#, tmp507
	movq	%rax, -768(%rbp)	# tmp507, MEM[(struct _Alloc_hider *)&mlcmd + 256B]._M_p
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movabsq	$7310874250647336307, %rax	#, tmp1016
	movq	%rax, -752(%rbp)	# tmp1016, MEM <char[1:10]> [(void *)&mlcmd + 272B]
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	-720(%rbp), %rax	#, tmp512
	movq	%rax, -736(%rbp)	# tmp512, MEM[(struct _Alloc_hider *)&mlcmd + 288B]._M_p
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	leaq	-1376(%rbp), %rax	#, tmp929
	movq	%rax, %rsi	# tmp929,
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -841(%rbp)	#, MEM[(char_type &)&mlcmd + 183]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movb	$110, -808(%rbp)	#, MEM <char[1:9]> [(void *)&mlcmd + 208B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$9, -824(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 192B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -807(%rbp)	#, MEM[(char_type &)&mlcmd + 217]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1802398060, -784(%rbp)	#, MEM <char[1:4]> [(void *)&mlcmd + 240B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$4, -792(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 224B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -780(%rbp)	#, MEM[(char_type &)&mlcmd + 244]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movw	%r15w, -744(%rbp)	#, MEM <char[1:10]> [(void *)&mlcmd + 272B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$10, -760(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 256B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -742(%rbp)	#, MEM[(char_type &)&mlcmd + 282]
# /usr/include/c++/11/bits/basic_string.tcc:215: 	size_type __dnew = static_cast<size_type>(std::distance(__beg, __end));
	movq	$16, -1376(%rbp)	#, MEM[(long unsigned int *)_1257]
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	movq	%rax, -1456(%rbp)	# tmp929, %sfp
.LEHB11:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT	#
.LEHE11:
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	-1376(%rbp), %rdx	# MEM[(long unsigned int *)_1257], MEM[(long unsigned int *)_1257]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$29285, %esi	#,
	movl	$28274, %edi	#,
	movdqa	.LC24(%rip), %xmm0	#, tmp951
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, -736(%rbp)	# _471, MEM[(struct basic_string *)&mlcmd + 288B]._M_dataplus._M_p
# /usr/include/c++/11/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	leaq	-320(%rbp), %r14	#, tmp933
	leaq	-576(%rbp), %rbx	#, _204
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	%rdx, -720(%rbp)	# MEM[(long unsigned int *)_1257], MEM[(struct basic_string *)&mlcmd + 288B].D.25367._M_allocated_capacity
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movups	%xmm0, (%rax)	# tmp951, MEM <char[1:16]> [(void *)_471]
# /usr/include/c++/11/bits/basic_string.tcc:232: 	_M_set_length(__dnew);
	movq	-1376(%rbp), %rax	# MEM[(long unsigned int *)_1257], __dnew.25_475
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movq	-736(%rbp), %rdx	# MEM[(const struct basic_string *)&mlcmd + 288B]._M_dataplus._M_p, MEM[(const struct basic_string *)&mlcmd + 288B]._M_dataplus._M_p
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movq	_Z21command_logic_clusterR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp536
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	%rax, -728(%rbp)	# __dnew.25_475, MEM[(struct basic_string *)&mlcmd + 288B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%rdx,%rax)	#, MEM[(char_type &)_477]
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	-688(%rbp), %rax	#, tmp519
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movhps	_Z20command_logic_defineR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp536
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	movq	%rax, -704(%rbp)	# tmp519, MEM[(struct _Alloc_hider *)&mlcmd + 320B]._M_p
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movabsq	$8678263220334848099, %rax	#, tmp1018
	movq	%rax, -688(%rbp)	# tmp1018, MEM <char[1:8]> [(void *)&mlcmd + 336B]
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	-656(%rbp), %rax	#, tmp524
	movq	%rax, -672(%rbp)	# tmp524, MEM[(struct _Alloc_hider *)&mlcmd + 352B]._M_p
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movabsq	$7959953343574405732, %rax	#, tmp1019
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movaps	%xmm0, -1152(%rbp)	# tmp536, MEM <vector(2) long unsigned int> [(void (*<T20e92>) (struct LDCProcessState &) *)&interpreter_behaviour]
	movq	_Z22command_logic_describeR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp541
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	%rax, -656(%rbp)	# tmp1019, MEM <char[1:8]> [(void *)&mlcmd + 368B]
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	-624(%rbp), %rax	#, tmp529
	movq	%rax, -640(%rbp)	# tmp529, MEM[(struct _Alloc_hider *)&mlcmd + 384B]._M_p
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movhps	_Z25command_logic_fattributesR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp541
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	-592(%rbp), %rax	#, tmp533
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movw	%si, -620(%rbp)	#, MEM <char[1:6]> [(void *)&mlcmd + 400B]
# /usr/include/c++/11/bits/basic_string.h:165: 	: allocator_type(__a), _M_p(__dat) { }
	movq	%rax, -608(%rbp)	# tmp533, MEM[(struct _Alloc_hider *)&mlcmd + 416B]._M_p
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movw	%di, -588(%rbp)	#, MEM <char[1:6]> [(void *)&mlcmd + 432B]
# /usr/include/c++/11/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movq	%r14, %rdi	# tmp933,
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$8, -696(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 320B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -680(%rbp)	#, MEM[(char_type &)&mlcmd + 344]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$8, -664(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 352B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -648(%rbp)	#, MEM[(char_type &)&mlcmd + 376]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1684630643, -624(%rbp)	#, MEM <char[1:6]> [(void *)&mlcmd + 400B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$6, -632(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 384B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -618(%rbp)	#, MEM[(char_type &)&mlcmd + 406]
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1970562418, -592(%rbp)	#, MEM <char[1:6]> [(void *)&mlcmd + 432B]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$6, -600(%rbp)	#, MEM[(struct basic_string *)&mlcmd + 416B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -586(%rbp)	#, MEM[(char_type &)&mlcmd + 438]
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movaps	%xmm0, -1136(%rbp)	# tmp541, MEM <vector(2) long unsigned int> [(void (*<T20e92>) (struct LDCProcessState &) *)&interpreter_behaviour + 16B]
	movq	_Z26command_logic_syntax_errorR15LDCProcessState@GOTPCREL(%rip), %rax	#, tmp572
	movq	_Z25command_logic_sattributesR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp546
# tmp_cmp.cpp:115: 	LDCProcessState state = {
	movq	$0, -1368(%rbp)	#, MEM <char[8]> [(struct LDCProcessState *)_1257]
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movhps	_Z21command_logic_segmentR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp546
	movq	%rax, -1040(%rbp)	# tmp572, interpreter_behaviour[14]
	movaps	%xmm0, -1120(%rbp)	# tmp546, MEM <vector(2) long unsigned int> [(void (*<T20e92>) (struct LDCProcessState &) *)&interpreter_behaviour + 32B]
	movq	_Z23command_logic_conditionR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp551
# tmp_cmp.cpp:118: 	};
	movq	%r12, -1376(%rbp)	# path, MEM[(struct LDCProcessState *)_1257].fpath
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movhps	_Z18command_logic_linkR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp551
# tmp_cmp.cpp:118: 	};
	movq	%r13, -1312(%rbp)	# rClusters, MEM[(struct LDCProcessState *)_1257].clusters
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movaps	%xmm0, -1104(%rbp)	# tmp551, MEM <vector(2) long unsigned int> [(void (*<T20e92>) (struct LDCProcessState &) *)&interpreter_behaviour + 48B]
	movq	_Z24command_logic_subsequentR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp556
# /usr/include/c++/11/bits/stl_vector.h:98: 	: _M_start(), _M_finish(), _M_end_of_storage()
	movq	$0, -1392(%rbp)	#, MEM[(struct _Vector_impl_data *)&cmd_buffer]._M_end_of_storage
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movhps	_Z26command_logic_sysbehaviourR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp556
# /usr/include/c++/11/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movq	%rbx, -1528(%rbp)	# _204, %sfp
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movaps	%xmm0, -1088(%rbp)	# tmp556, MEM <vector(2) long unsigned int> [(void (*<T20e92>) (struct LDCProcessState &) *)&interpreter_behaviour + 64B]
	movq	_Z22command_logic_checkboxR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp561
# /usr/include/c++/11/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movq	%r14, -1560(%rbp)	# tmp933, %sfp
# tmp_cmp.cpp:106: 	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
	movhps	_Z22command_logic_dropdownR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp561
	movaps	%xmm0, -1072(%rbp)	# tmp561, MEM <vector(2) long unsigned int> [(void (*<T20e92>) (struct LDCProcessState &) *)&interpreter_behaviour + 80B]
	movq	_Z20command_logic_sliderR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp566
	movhps	_Z20command_logic_returnR15LDCProcessState@GOTPCREL(%rip), %xmm0	#, tmp566
	movaps	%xmm0, -1056(%rbp)	# tmp566, MEM <vector(2) long unsigned int> [(void (*<T20e92>) (struct LDCProcessState &) *)&interpreter_behaviour + 96B]
# /usr/include/c++/11/bits/stl_vector.h:98: 	: _M_start(), _M_finish(), _M_end_of_storage()
	pxor	%xmm0, %xmm0	# tmp573
	movaps	%xmm0, -1360(%rbp)	# tmp573, MEM <vector(2) long unsigned int> [(void *)_1257]
	movaps	%xmm0, -1344(%rbp)	# tmp573, MEM <vector(2) long unsigned int> [(void *)_1257]
	movaps	%xmm0, -1328(%rbp)	# tmp573, MEM <vector(2) long unsigned int> [(void *)_1257]
	movaps	%xmm0, -1408(%rbp)	# tmp573, MEM <vector(2) long unsigned int> [(struct ListLanguageCommand * *)&cmd_buffer]
# /usr/include/c++/11/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	call	_ZNSt8ios_baseC2Ev@PLT	#
# /usr/include/c++/11/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	pxor	%xmm0, %xmm0	# tmp573
# /usr/include/c++/11/bits/basic_ios.h:461:       : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false), 
	xorl	%r8d, %r8d	#
# /usr/include/c++/11/istream:608:       { this->init(0); }
	xorl	%esi, %esi	#
# /usr/include/c++/11/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	leaq	16+_ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax	#, tmp1029
# /usr/include/c++/11/bits/basic_ios.h:461:       : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false), 
	movw	%r8w, -96(%rbp)	#, MEM <unsigned short> [(void *)&file + 480B]
# /usr/include/c++/11/istream:607:       : _M_gcount(streamsize(0))
	movq	16+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rcx	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 16B], tmp1032
# /usr/include/c++/11/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movq	%rax, -320(%rbp)	# tmp1029, MEM[(struct basic_ios *)&file + 256B].D.41295._vptr.ios_base
# /usr/include/c++/11/istream:607:       : _M_gcount(streamsize(0))
	movq	8+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rax	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 8B], tmp1030
# /usr/include/c++/11/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movups	%xmm0, -88(%rbp)	# tmp573, MEM <vector(2) long unsigned int> [(void *)&file + 488B]
	movups	%xmm0, -72(%rbp)	# tmp573, MEM <vector(2) long unsigned int> [(void *)&file + 504B]
# /usr/include/c++/11/istream:607:       : _M_gcount(streamsize(0))
	movq	%rax, -576(%rbp)	# tmp1030, MEM[(struct basic_istream *)&file]._vptr.basic_istream
	movq	-24(%rax), %rax	# MEM[(long int *)_532 + -24B], MEM[(long int *)_532 + -24B]
# /usr/include/c++/11/bits/basic_ios.h:461:       : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false), 
	movq	$0, -104(%rbp)	#, MEM[(struct basic_ios *)&file + 256B]._M_tie
# /usr/include/c++/11/istream:607:       : _M_gcount(streamsize(0))
	movq	%rcx, -576(%rbp,%rax)	# tmp1032, MEM[(struct basic_ios *)_535].D.41295._vptr.ios_base
# /usr/include/c++/11/istream:608:       { this->init(0); }
	movq	8+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rcx	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 8B], tmp1033
# /usr/include/c++/11/istream:607:       : _M_gcount(streamsize(0))
	movq	$0, -568(%rbp)	#, MEM[(struct basic_istream *)&file]._M_gcount
# /usr/include/c++/11/istream:608:       { this->init(0); }
	movq	-24(%rcx), %rax	# MEM[(long int *)_532 + -24B], tmp586
	addq	%rbx, %rax	# _204, tmp586
	movq	%rax, %rdi	# tmp586, tmp586
.LEHB12:
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E@PLT	#
.LEHE12:
# /usr/include/c++/11/fstream:535:       : __istream_type(), _M_filebuf()
	leaq	24+_ZTVSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rax	#, tmp1035
	movq	%rax, -576(%rbp)	# tmp1035, MEM[(struct basic_ifstream *)&file].D.49057._vptr.basic_istream
	addq	$40, %rax	#, tmp1036
	movq	%rax, -320(%rbp)	# tmp1036, MEM[(struct basic_ios *)&file + 256B].D.41295._vptr.ios_base
# /usr/include/c++/11/fstream:535:       : __istream_type(), _M_filebuf()
	leaq	-560(%rbp), %rax	#, tmp925
	movq	%rax, %rdi	# tmp925,
	movq	%rax, -1544(%rbp)	# tmp925, %sfp
	movq	%rax, %r15	# tmp925, tmp925
.LEHB13:
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEEC1Ev@PLT	#
.LEHE13:
# /usr/include/c++/11/fstream:537: 	this->init(&_M_filebuf);
	movq	%r15, %rsi	# tmp925,
	movq	%r14, %rdi	# tmp933,
.LEHB14:
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E@PLT	#
# /usr/include/c++/11/fstream:665: 	if (!_M_filebuf.open(__s, __mode | ios_base::in))
	movl	$8, %edx	#,
	movq	%r12, %rsi	# path,
	movq	%r15, %rdi	# tmp925,
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEE4openEPKcSt13_Ios_Openmode@PLT	#
# /usr/include/c++/11/fstream:666: 	  this->setstate(ios_base::failbit);
	movq	-576(%rbp), %rdx	# file.D.49057._vptr.basic_istream, file.D.49057._vptr.basic_istream
	addq	-24(%rdx), %rbx	# MEM[(long int *)pretmp_351 + -24B], _203
	movq	%rbx, %rdi	# _203, _203
# /usr/include/c++/11/fstream:665: 	if (!_M_filebuf.open(__s, __mode | ios_base::in))
	testq	%rax, %rax	# tmp955
	je	.L339	#,
# /usr/include/c++/11/fstream:670: 	  this->clear();
	xorl	%esi, %esi	#
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT	#
.LEHE14:
# tmp_cmp.cpp:124: 	while (!file.eof()) {
	testb	$2, -288(%rbp)	#, MEM[(const struct basic_ios *)&file + 256B].D.41295._M_streambuf_state
	jne	.L340	#,
.L251:
	leaq	-1280(%rbp), %rax	#, tmp932
	movl	$1, %ecx	#,
	movq	$0, -1488(%rbp)	#, %sfp
	movq	%rax, -1432(%rbp)	# tmp932, %sfp
	leaq	-1024(%rbp), %rax	#, tmp926
	movq	%rax, -1520(%rbp)	# tmp926, %sfp
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	leaq	-1264(%rbp), %rax	#, tmp949
# tmp_cmp.cpp:124: 	while (!file.eof()) {
	movq	$0, -1480(%rbp)	#, %sfp
	movw	%cx, -1440(%rbp)	#, %sfp
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%rax, -1512(%rbp)	# tmp949, %sfp
	.p2align 4,,10
	.p2align 3
.L115:
	movq	-1432(%rbp), %rax	# %sfp, tmp932
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -1280(%rbp)	#, MEM[(char_type &)&line + 16]
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$0, -1288(%rbp)	#, MEM[(struct basic_string *)&line]._M_string_length
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%rax, -1296(%rbp)	# tmp932, MEM[(struct _Alloc_hider *)&line]._M_p
# /usr/include/c++/11/bits/basic_string.h:6573:     { return std::getline(__is, __str, __is.widen('\n')); }
	movq	-576(%rbp), %rax	# file._vptr.basic_istream, file._vptr.basic_istream
	movq	-24(%rax), %rax	# MEM[(long int *)_559 + -24B], MEM[(long int *)_559 + -24B]
	movq	-336(%rbp,%rax), %r12	# MEM[(const struct __ctype_type * *)_562 + 240B], _566
# /usr/include/c++/11/bits/basic_ios.h:49:       if (!__f)
	testq	%r12, %r12	# _566
	je	.L341	#,
# /usr/include/c++/11/bits/locale_facets.h:877: 	if (_M_widen_ok)
	cmpb	$0, 56(%r12)	#, MEM[(const struct ctype *)_566]._M_widen_ok
	je	.L123	#,
# /usr/include/c++/11/bits/basic_string.h:6573:     { return std::getline(__is, __str, __is.widen('\n')); }
	movsbl	67(%r12), %edx	# MEM[(const struct ctype *)_566]._M_widen[10], _470
.L124:
	leaq	-1296(%rbp), %r13	#, tmp930
	movq	-1528(%rbp), %rdi	# %sfp,
	movq	%r13, %rsi	# tmp930,
.LEHB15:
	call	_ZSt7getlineIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RNSt7__cxx1112basic_stringIS4_S5_T1_EES4_@PLT	#
.LEHE15:
# tmp_cmp.cpp:129: 		if (!line.rfind(':',0)) {
	xorl	%edx, %edx	#
	movl	$58, %esi	#,
	movq	%r13, %rdi	# tmp930,
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5rfindEcm@PLT	#
# tmp_cmp.cpp:129: 		if (!line.rfind(':',0)) {
	testq	%rax, %rax	# tmp960
	je	.L342	#,
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1296(%rbp), %r13	# MEM[(const struct basic_string *)&line]._M_dataplus._M_p, _662
# /usr/include/c++/11/bits/basic_string.h:927:       { return _M_string_length; }
	movq	-1288(%rbp), %r12	# MEM[(const struct basic_string *)&line]._M_string_length, prephitmp_482
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	leaq	-1216(%rbp), %rbx	#, tmp936
	movq	%rbx, -1232(%rbp)	# tmp936, MEM[(struct _Alloc_hider *)_1253]._M_p
# /usr/include/c++/11/bits/basic_string.tcc:211: 	if (__gnu_cxx::__is_null_pointer(__beg) && __beg != __end)
	movq	%r13, %rax	# _662, tmp981
	addq	%r12, %rax	# prephitmp_482, tmp981
	je	.L160	#,
	testq	%r13, %r13	# _662
	je	.L343	#,
.L160:
# /usr/include/c++/11/bits/basic_string.tcc:215: 	size_type __dnew = static_cast<size_type>(std::distance(__beg, __end));
	movq	%r12, -1416(%rbp)	# prephitmp_482, MEM[(long unsigned int *)_1255]
# /usr/include/c++/11/bits/basic_string.tcc:217: 	if (__dnew > size_type(_S_local_capacity))
	cmpq	$15, %r12	#, prephitmp_482
	ja	.L344	#,
# /usr/include/c++/11/bits/basic_string.h:356: 	if (__n == 1)
	cmpq	$1, %r12	#, prephitmp_482
	jne	.L163	#,
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movzbl	0(%r13), %eax	# MEM[(const char_type &)_662], _675
	leaq	-1232(%rbp), %r14	#, tmp935
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	%al, -1216(%rbp)	# _675, MEM[(char_type &)_1253]
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movq	%rbx, %rax	# tmp936, pretmp_483
.L164:
# /usr/include/c++/11/bits/basic_string.h:1273:       { return _M_replace_aux(this->size(), size_type(0), __n, __c); }
	movl	$32, %r8d	#,
	movl	$1, %ecx	#,
	xorl	%edx, %edx	#
	movq	%r14, %rdi	# tmp935,
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	%r12, -1224(%rbp)	# prephitmp_482, MEM[(struct basic_string *)_1253]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%rax,%r12)	#, MEM[(char_type &)_678]
# /usr/include/c++/11/bits/basic_string.h:1273:       { return _M_replace_aux(this->size(), size_type(0), __n, __c); }
	movq	-1224(%rbp), %rsi	# MEM[(struct basic_string *)_1253]._M_string_length,
.LEHB16:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE14_M_replace_auxEmmmc@PLT	#
.LEHE16:
# tmp_cmp.cpp:143: 		} else cmd_buffer.back().buffer += line+' ';
	movq	-1480(%rbp), %rax	# %sfp, prephitmp_466
# /usr/include/c++/11/bits/basic_string.h:1214:       { return _M_append(__str._M_data(), __str.size()); }
	movq	-1224(%rbp), %rdx	# MEM[(struct basic_string *)_1253]._M_string_length,
	movq	-1232(%rbp), %rsi	# MEM[(struct basic_string *)_1253]._M_dataplus._M_p,
# tmp_cmp.cpp:143: 		} else cmd_buffer.back().buffer += line+' ';
	leaq	-40(%rax), %rdi	#, tmp740
.LEHB17:
# /usr/include/c++/11/bits/basic_string.h:1214:       { return _M_append(__str._M_data(), __str.size()); }
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcm@PLT	#
.LEHE17:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1232(%rbp), %rdi	# MEM[(struct basic_string *)_1253]._M_dataplus._M_p, _153
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	%rbx, %rdi	# tmp936, _153
	je	.L159	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	-1216(%rbp), %rax	# MEM[(struct basic_string *)_1253].D.25367._M_allocated_capacity, tmp1105
	leaq	1(%rax), %rsi	#, tmp743
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L159:
# tmp_cmp.cpp:146: 		line_number++;
	addw	$1, -1440(%rbp)	#, %sfp
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1296(%rbp), %rdi	# line._M_dataplus._M_p, _152
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	-1432(%rbp), %rdi	# %sfp, _152
	je	.L170	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	-1280(%rbp), %rax	# line.D.25367._M_allocated_capacity, tmp1108
	leaq	1(%rax), %rsi	#, tmp747
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# tmp_cmp.cpp:124: 	while (!file.eof()) {
	testb	$2, -288(%rbp)	#, MEM[(const struct basic_ios *)&file + 256B].D.41295._M_streambuf_state
	je	.L115	#,
.L172:
# /usr/include/c++/11/bits/stl_iterator.h:1028:       : _M_current(__i) { }
	movq	-1408(%rbp), %rax	# MEM[(struct ListLanguageCommand * const &)&cmd_buffer], pretmp_171
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	-1488(%rbp), %rcx	# %sfp, prephitmp_469
	subq	%rax, %rcx	# pretmp_171, prephitmp_469
# /usr/include/c++/11/bits/stl_iterator.h:1028:       : _M_current(__i) { }
	movq	%rax, -1536(%rbp)	# pretmp_171, %sfp
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	%rcx, -1552(%rbp)	# prephitmp_469, %sfp
.L116:
# /usr/include/c++/11/fstream:737: 	if (!_M_filebuf.close())
	movq	-1544(%rbp), %rdi	# %sfp,
.LEHB18:
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEE5closeEv@PLT	#
# /usr/include/c++/11/fstream:737: 	if (!_M_filebuf.close())
	testq	%rax, %rax	# tmp969
	je	.L345	#,
.L173:
	movq	-1536(%rbp), %rax	# %sfp, pretmp_171
# tmp_cmp.cpp:150: 	for (ListLanguageCommand &cmd : cmd_buffer) {
	movq	-1480(%rbp), %rcx	# %sfp, prephitmp_466
	movq	%rax, %rbx	# pretmp_171, ivtmp.580
	cmpq	%rcx, %rax	# prephitmp_466, pretmp_171
	je	.L178	#,
	.p2align 4,,10
	.p2align 3
.L177:
# tmp_cmp.cpp:151: 		state.cmd = &cmd;
	movq	%rbx, -1368(%rbp)	# ivtmp.580, MEM[(struct LDCProcessState *)_1257].cmd
# tmp_cmp.cpp:152: 		interpreter_behaviour[cmd.id](state);
	movzbl	(%rbx), %eax	# MEM[(unsigned char *)SR.365_710], MEM[(unsigned char *)SR.365_710]
# tmp_cmp.cpp:152: 		interpreter_behaviour[cmd.id](state);
	movq	-1456(%rbp), %rdi	# %sfp,
	call	*-1152(%rbp,%rax,8)	# interpreter_behaviour[_14]
# tmp_cmp.cpp:150: 	for (ListLanguageCommand &cmd : cmd_buffer) {
	addq	$80, %rbx	#, ivtmp.580
	cmpq	%rbx, -1480(%rbp)	# ivtmp.580, %sfp
	jne	.L177	#,
.L178:
# tmp_cmp.cpp:156: 	for (uint16_t i=0;i<state.clusters.size();i++) {
	movq	-1312(%rbp), %rax	# MEM[(struct LDCProcessState *)_1257].clusters, _859
# tmp_cmp.cpp:156: 	for (uint16_t i=0;i<state.clusters.size();i++) {
	xorl	%edx, %edx	#
# tmp_cmp.cpp:156: 	for (uint16_t i=0;i<state.clusters.size();i++) {
	movq	$0, -1496(%rbp)	#, %sfp
# tmp_cmp.cpp:156: 	for (uint16_t i=0;i<state.clusters.size();i++) {
	movw	%dx, -1488(%rbp)	#, %sfp
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	8(%rax), %rcx	# MEM[(const struct vector *)_859].D.297580._M_impl.D.296925._M_finish, prephitmp_660
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	(%rax), %r14	# MEM[(const struct vector *)_859].D.297580._M_impl.D.296925._M_start, prephitmp_3
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	%rcx, -1504(%rbp)	# prephitmp_660, %sfp
# tmp_cmp.cpp:156: 	for (uint16_t i=0;i<state.clusters.size();i++) {
	cmpq	%rcx, %r14	# prephitmp_660, prephitmp_3
	je	.L176	#,
	.p2align 4,,10
	.p2align 3
.L175:
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	-1496(%rbp), %rcx	# %sfp, _35
	leaq	0(,%rcx,8), %rax	#, tmp770
	subq	%rcx, %rax	# _35, tmp771
	salq	$4, %rax	#, tmp771
	movq	%rax, -1472(%rbp)	# tmp772, %sfp
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	leaq	(%r14,%rax), %r13	#, _249
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	leaq	(%rcx,%rcx,2), %rax	#, tmp775
	salq	$4, %rax	#, tmp776
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	addq	-1360(%rbp), %rax	# MEM[(struct vector *)_1257].D.279045._M_impl.D.278390._M_start, _212
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	(%rax), %rcx	# MEM[(const struct vector *)_212].D.211637._M_impl.D.210987._M_start, _21
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	8(%rax), %rdx	# MEM[(const struct vector *)_212].D.211637._M_impl.D.210987._M_finish, tmp778
	subq	%rcx, %rdx	# _21, tmp778
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	%rcx, -1440(%rbp)	# _21, %sfp
# tmp_cmp.cpp:160: 		for (uint16_t j=0;j<state.crefs[i].parent_ids.size();j++) {
	sarq	%rdx	# tmp778
	movq	%rdx, -1448(%rbp)	# tmp778, %sfp
	je	.L180	#,
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	24(%rax), %rax	# MEM[(struct vector *)_212 + 24B].D.274826._M_impl.D.274171._M_start, _142
	movq	%r13, -1512(%rbp)	# _249, %sfp
# tmp_cmp.cpp:160: 		for (uint16_t j=0;j<state.crefs[i].parent_ids.size();j++) {
	xorl	%ebx, %ebx	# _20
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	%rax, -1464(%rbp)	# _142, %sfp
	movq	32(%r13), %rax	# MEM[(struct vector *)_172 + 32B].D.276944._M_impl.D.276289._M_start, _116
	movq	%r14, %r13	# prephitmp_3, prephitmp_3
	movq	%rax, -1456(%rbp)	# _116, %sfp
# tmp_cmp.cpp:160: 		for (uint16_t j=0;j<state.crefs[i].parent_ids.size();j++) {
	xorl	%eax, %eax	#
	movw	%ax, -1432(%rbp)	#, %sfp
	.p2align 4,,10
	.p2align 3
.L181:
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	%rbx, %rax	# _20, tmp797
# tmp_cmp.cpp:161: 			uint16_t k = 0;
	xorl	%r14d, %r14d	# k
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	salq	$5, %rax	#, tmp797
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	addq	-1464(%rbp), %rax	# %sfp, _140
	movq	(%rax), %r12	# MEM[(char * *)_140], _342
	movq	8(%rax), %r15	# MEM[(long unsigned int *)_140 + 8B], _343
	jmp	.L184	#
	.p2align 4,,10
	.p2align 3
.L182:
# tmp_cmp.cpp:162: 			while (state.clusters[k].id!=state.crefs[i].child_names[j]) k++;
	addl	$1, %r14d	#, k
.L184:
# tmp_cmp.cpp:162: 			while (state.clusters[k].id!=state.crefs[i].child_names[j]) k++;
	movzwl	%r14w, %edx	# k, k
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	leaq	0(,%rdx,8), %rax	#, tmp783
	subq	%rdx, %rax	# k, tmp784
	salq	$4, %rax	#, tmp785
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	addq	%r13, %rax	# prephitmp_3, _151
# /usr/include/c++/11/bits/basic_string.h:6237: 	      && !std::char_traits<_CharT>::compare(__lhs.data(), __rhs.data(),
	cmpq	%r15, 8(%rax)	# _343, MEM[(long unsigned int *)_151 + 8B]
	jne	.L182	#,
	movq	(%rax), %rdi	# MEM[(char * *)_151], _340
# /usr/include/c++/11/bits/char_traits.h:374: 	if (__n == 0)
	testq	%r15, %r15	# _343
	je	.L183	#,
# /usr/include/c++/11/bits/char_traits.h:389: 	return __builtin_memcmp(__s1, __s2, __n);
	movq	%r15, %rdx	# _343,
	movq	%r12, %rsi	# _342,
	call	memcmp@PLT	#
# /usr/include/c++/11/bits/basic_string.h:6237: 	      && !std::char_traits<_CharT>::compare(__lhs.data(), __rhs.data(),
	testl	%eax, %eax	# tmp970
	jne	.L182	#,
.L183:
# tmp_cmp.cpp:163: 			state.clusters[i].elist[state.crefs[i].parent_ids[j]].tdata = k;
	movq	-1440(%rbp), %rax	# %sfp, _21
# tmp_cmp.cpp:163: 			state.clusters[i].elist[state.crefs[i].parent_ids[j]].tdata = k;
	movq	-1456(%rbp), %rcx	# %sfp, _116
# tmp_cmp.cpp:160: 		for (uint16_t j=0;j<state.crefs[i].parent_ids.size();j++) {
	addw	$1, -1432(%rbp)	#, %sfp
# tmp_cmp.cpp:163: 			state.clusters[i].elist[state.crefs[i].parent_ids[j]].tdata = k;
	movzwl	(%rax,%rbx,2), %eax	# *_107, *_107
# tmp_cmp.cpp:160: 		for (uint16_t j=0;j<state.crefs[i].parent_ids.size();j++) {
	movzwl	-1432(%rbp), %ebx	# %sfp,
# tmp_cmp.cpp:163: 			state.clusters[i].elist[state.crefs[i].parent_ids[j]].tdata = k;
	leaq	(%rax,%rax,4), %rdx	#, tmp792
	leaq	(%rax,%rdx,4), %rax	#, tmp794
	movw	%r14w, 104(%rcx,%rax,8)	# k, _112->tdata
# tmp_cmp.cpp:160: 		for (uint16_t j=0;j<state.crefs[i].parent_ids.size();j++) {
	cmpq	-1448(%rbp), %rbx	# %sfp, _20
	jb	.L181	#,
	movq	%r13, %r14	# prephitmp_3, prephitmp_3
	movq	-1512(%rbp), %r13	# %sfp, _249
.L180:
# tmp_cmp.cpp:167: 		for (uint16_t j=0;j<cc.linked_ids.size();j++) {
	movq	80(%r13), %rax	# MEM[(const struct vector *)_172 + 80B].D.211637._M_impl.D.210987._M_start, tmp1136
	cmpq	%rax, 88(%r13)	# tmp1136, MEM[(const struct vector *)_172 + 80B].D.211637._M_impl.D.210987._M_finish
	je	.L185	#,
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	-1496(%rbp), %rax	# %sfp, _35
# tmp_cmp.cpp:167: 		for (uint16_t j=0;j<cc.linked_ids.size();j++) {
	xorl	%r12d, %r12d	# j
# tmp_cmp.cpp:167: 		for (uint16_t j=0;j<cc.linked_ids.size();j++) {
	xorl	%ebx, %ebx	# _30
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	leaq	(%rax,%rax,2), %r15	#, tmp801
	salq	$3, %r15	#, tmp802
	jmp	.L188	#
	.p2align 4,,10
	.p2align 3
.L186:
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	-1312(%rbp), %rdx	# MEM[(struct LDCProcessState *)_1257].clusters, MEM[(struct LDCProcessState *)_1257].clusters
	movq	-1472(%rbp), %rcx	# %sfp, _240
	addq	(%rdx), %rcx	# _23->D.297580._M_impl.D.296925._M_start, _240
# tmp_cmp.cpp:176: 			LDCEntity &ce = state.clusters[i].elist[state.clusters[i].linked_ids[j]];
	movq	80(%rcx), %rdx	# MEM[(struct vector *)_240 + 80B].D.211637._M_impl.D.210987._M_start, MEM[(struct vector *)_240 + 80B].D.211637._M_impl.D.210987._M_start
	movzwl	(%rdx,%rbx,2), %edx	# *_238, *_238
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	leaq	(%rdx,%rdx,4), %rsi	#, tmp817
	leaq	(%rdx,%rsi,4), %rsi	#, tmp819
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	32(%rcx), %rdx	# MEM[(struct vector *)_240 + 32B].D.276944._M_impl.D.276289._M_start, MEM[(struct vector *)_240 + 32B].D.276944._M_impl.D.276289._M_start
# tmp_cmp.cpp:178: 			ce.tdata = Init::iConfig[ivar];
	leaq	_ZN4Init7iConfigE(%rip), %rcx	#, tmp1140
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	leaq	(%rdx,%rsi,8), %rdx	#, _235
# tmp_cmp.cpp:177: 			ce.vlink = ivar;
	movw	%ax, 96(%rdx)	# _160, MEM[(struct LDCEntity &)_235].vlink
# tmp_cmp.cpp:178: 			ce.tdata = Init::iConfig[ivar];
	movl	%eax, %eax	# _160, _160
# tmp_cmp.cpp:178: 			ce.tdata = Init::iConfig[ivar];
	movl	(%rcx,%rax,4), %eax	# iConfig[_160], iConfig[_160]
	movw	%ax, 104(%rdx)	# iConfig[_160], MEM[(struct LDCEntity &)_235].tdata
.L187:
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	88(%r13), %rax	# MEM[(const struct vector *)_172 + 80B].D.211637._M_impl.D.210987._M_finish, MEM[(const struct vector *)_172 + 80B].D.211637._M_impl.D.210987._M_finish
# tmp_cmp.cpp:167: 		for (uint16_t j=0;j<cc.linked_ids.size();j++) {
	addl	$1, %r12d	#, j
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	subq	80(%r13), %rax	# MEM[(const struct vector *)_172 + 80B].D.211637._M_impl.D.210987._M_start, tmp825
# tmp_cmp.cpp:167: 		for (uint16_t j=0;j<cc.linked_ids.size();j++) {
	movzwl	%r12w, %ebx	# j, _30
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	sarq	%rax	# tmp828
# tmp_cmp.cpp:167: 		for (uint16_t j=0;j<cc.linked_ids.size();j++) {
	cmpq	%rax, %rbx	# tmp828, _30
	jnb	.L346	#,
.L188:
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	-1336(%rbp), %rax	# MEM[(struct vector *)_1257].D.280086._M_impl.D.279431._M_start, MEM[(struct vector *)_1257].D.280086._M_impl.D.279431._M_start
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	%rbx, %r14	# _30, _79
	salq	$5, %r14	#, _79
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	(%rax,%r15), %rax	# MEM[(struct vector *)_71].D.274826._M_impl.D.274171._M_start, MEM[(struct vector *)_71].D.274826._M_impl.D.274171._M_start
# tmp_cmp.cpp:168: 			uint32_t ivar = Init::find_iKey(state.srefs[i][j].c_str());
	movq	(%rax,%r14), %rdi	# MEM[(const struct basic_string *)_77]._M_dataplus._M_p, MEM[(const struct basic_string *)_77]._M_dataplus._M_p
	call	_ZN4Init9find_iKeyEPKc@PLT	#
# tmp_cmp.cpp:169: 			if (ivar==InitVariable::VARIABLE_ERROR) {
	cmpl	$16, %eax	#, _160
	jne	.L186	#,
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	-1336(%rbp), %rax	# MEM[(struct vector *)_1257].D.280086._M_impl.D.279431._M_start, MEM[(struct vector *)_1257].D.280086._M_impl.D.279431._M_start
# /usr/include/x86_64-linux-gnu/bits/stdio2.h:112:   return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
	movq	-1376(%rbp), %rdx	# MEM[(struct LDCProcessState *)_1257].fpath,
	movl	$1, %edi	#,
	leaq	.LC23(%rip), %rsi	#,
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	(%rax,%r15), %rax	# MEM[(struct vector *)_232].D.274826._M_impl.D.274171._M_start, MEM[(struct vector *)_232].D.274826._M_impl.D.274171._M_start
# /usr/include/x86_64-linux-gnu/bits/stdio2.h:112:   return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
	movq	(%rax,%r14), %rcx	# MEM[(const struct basic_string *)_230]._M_dataplus._M_p, MEM[(const struct basic_string *)_230]._M_dataplus._M_p
	xorl	%eax, %eax	#
	call	__printf_chk@PLT	#
.LEHE18:
	jmp	.L187	#
	.p2align 4,,10
	.p2align 3
.L346:
# tmp_cmp.cpp:182: 		for (LDCSegment &segment : state.clusters[i].slist)
	movq	-1312(%rbp), %rax	# MEM[(struct LDCProcessState *)_1257].clusters, pretmp_37
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	(%rax), %r14	# pretmp_37->D.297580._M_impl.D.296925._M_start, prephitmp_3
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	8(%rax), %rax	# MEM[(const struct vector *)pretmp_37].D.297580._M_impl.D.296925._M_finish, prephitmp_660
	movq	%rax, -1504(%rbp)	# prephitmp_660, %sfp
	movq	-1472(%rbp), %rax	# %sfp, tmp772
	leaq	(%r14,%rax), %r13	#, _249
.L185:
# /usr/include/c++/11/bits/stl_iterator.h:1028:       : _M_current(__i) { }
	movq	56(%r13), %rax	# MEM[(struct LDCSegment * const &)_249 + 56], _247
	movq	64(%r13), %rsi	# MEM[(struct LDCSegment * const &)_249 + 64], _246
# tmp_cmp.cpp:182: 		for (LDCSegment &segment : state.clusters[i].slist)
	cmpq	%rax, %rsi	# _247, _246
	je	.L191	#,
# /usr/include/c++/11/bits/stl_vector.h:1046: 	return *(this->_M_impl._M_start + __n);
	movq	32(%r13), %rdi	# MEM[(struct vector *)_249 + 32B].D.276944._M_impl.D.276289._M_start, _251
	.p2align 4,,10
	.p2align 3
.L190:
# tmp_cmp.cpp:183: 			state.clusters[i].elist[segment.position].jsegment = true;
	movzwl	(%rax), %edx	# MEM[(short unsigned int *)_983], MEM[(short unsigned int *)_983]
# tmp_cmp.cpp:182: 		for (LDCSegment &segment : state.clusters[i].slist)
	addq	$40, %rax	#, ivtmp.559
# tmp_cmp.cpp:183: 			state.clusters[i].elist[segment.position].jsegment = true;
	leaq	(%rdx,%rdx,4), %rcx	#, tmp838
	leaq	(%rdx,%rcx,4), %rdx	#, tmp840
	movb	$1, 160(%rdi,%rdx,8)	#, _253->jsegment
# tmp_cmp.cpp:182: 		for (LDCSegment &segment : state.clusters[i].slist)
	cmpq	%rax, %rsi	# ivtmp.559, _246
	jne	.L190	#,
.L191:
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	-1504(%rbp), %rax	# %sfp, tmp829
# tmp_cmp.cpp:156: 	for (uint16_t i=0;i<state.clusters.size();i++) {
	addw	$1, -1488(%rbp)	#, %sfp
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movabsq	$7905747460161236407, %rsi	#, tmp1147
# tmp_cmp.cpp:156: 	for (uint16_t i=0;i<state.clusters.size();i++) {
	movzwl	-1488(%rbp), %ecx	# %sfp,
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	subq	%r14, %rax	# prephitmp_3, tmp829
	sarq	$4, %rax	#, tmp831
# tmp_cmp.cpp:156: 	for (uint16_t i=0;i<state.clusters.size();i++) {
	movq	%rcx, -1496(%rbp)	# _35, %sfp
# /usr/include/c++/11/bits/stl_vector.h:919:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	imulq	%rsi, %rax	# tmp1147, tmp832
# tmp_cmp.cpp:156: 	for (uint16_t i=0;i<state.clusters.size();i++) {
	cmpq	%rax, %rcx	# tmp832, _35
	jb	.L175	#,
.L176:
# /usr/include/c++/11/fstream:605:       { }
	leaq	24+_ZTVSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rax	#, tmp1118
# /usr/include/c++/11/fstream:254: 	  { this->close(); }
	movq	-1544(%rbp), %rdi	# %sfp,
# /usr/include/c++/11/fstream:605:       { }
	movq	%rax, -576(%rbp)	# tmp1118, file.D.49057._vptr.basic_istream
	addq	$40, %rax	#, tmp1119
	movq	%rax, -320(%rbp)	# tmp1119, MEM[(struct basic_ios *)&file + 256B].D.41295._vptr.ios_base
# /usr/include/c++/11/fstream:257:       }
	leaq	16+_ZTVSt13basic_filebufIcSt11char_traitsIcEE(%rip), %rax	#, tmp1120
	movq	%rax, -560(%rbp)	# tmp1120, MEM[(struct basic_filebuf *)&file + 16B].D.48865._vptr.basic_streambuf
.LEHB19:
# /usr/include/c++/11/fstream:254: 	  { this->close(); }
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEE5closeEv@PLT	#
.LEHE19:
.L179:
# /usr/include/c++/11/fstream:257:       }
	leaq	-456(%rbp), %rdi	#, tmp844
	call	_ZNSt12__basic_fileIcED1Ev@PLT	#
# /usr/include/c++/11/streambuf:203:       { }
	leaq	16+_ZTVSt15basic_streambufIcSt11char_traitsIcEE(%rip), %rax	#, tmp1149
	leaq	-504(%rbp), %rdi	#, tmp847
	movq	%rax, -560(%rbp)	# tmp1149, MEM[(struct basic_streambuf *)&file + 16B]._vptr.basic_streambuf
	call	_ZNSt6localeD1Ev@PLT	#
# /usr/include/c++/11/istream:104:       { _M_gcount = streamsize(0); }
	movq	8+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rax	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 8B], tmp1150
	movq	16+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rcx	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 16B], tmp1152
# /usr/include/c++/11/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	-1560(%rbp), %rdi	# %sfp,
# /usr/include/c++/11/istream:104:       { _M_gcount = streamsize(0); }
	movq	%rax, -576(%rbp)	# tmp1150, MEM[(struct basic_istream *)&file]._vptr.basic_istream
	movq	-24(%rax), %rax	# MEM[(long int *)_532 + -24B], MEM[(long int *)_532 + -24B]
	movq	%rcx, -576(%rbp,%rax)	# tmp1152, MEM[(struct basic_ios *)_722].D.41295._vptr.ios_base
# /usr/include/c++/11/bits/basic_ios.h:282:       ~basic_ios() { }
	leaq	16+_ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax	#, tmp1153
	movq	%rax, -320(%rbp)	# tmp1153, MEM[(struct basic_ios *)&file + 256B].D.41295._vptr.ios_base
# /usr/include/c++/11/istream:104:       { _M_gcount = streamsize(0); }
	movq	$0, -568(%rbp)	#, MEM[(struct basic_istream *)&file]._M_gcount
# /usr/include/c++/11/bits/basic_ios.h:282:       ~basic_ios() { }
	call	_ZNSt8ios_baseD2Ev@PLT	#
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	movq	-1536(%rbp), %rax	# %sfp, pretmp_171
	movq	-1480(%rbp), %rcx	# %sfp, prephitmp_466
	movq	%rax, %rbx	# pretmp_171, __first
	cmpq	%rcx, %rax	# prephitmp_466, pretmp_171
	je	.L199	#,
	.p2align 4,,10
	.p2align 3
.L192:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	40(%rbx), %rdi	# MEM[(char * *)__first_502 + 40B], _728
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	56(%rbx), %rax	#, tmp853
	cmpq	%rax, %rdi	# tmp853, _728
	je	.L196	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	56(%rbx), %rax	# MEM <size_type> [(union ._anon_79 *)__first_502 + 56B], tmp1157
	leaq	1(%rax), %rsi	#, tmp854
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L196:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	8(%rbx), %rdi	# MEM[(char * *)__first_502 + 8B], _732
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	24(%rbx), %rax	#, tmp856
	cmpq	%rax, %rdi	# tmp856, _732
	je	.L197	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	24(%rbx), %rax	# MEM <size_type> [(union ._anon_79 *)__first_502 + 24B], tmp1158
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$80, %rbx	#, __first
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	leaq	1(%rax), %rsi	#, tmp857
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%rbx, -1480(%rbp)	# __first, %sfp
	jne	.L192	#,
.L199:
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	cmpq	$0, -1536(%rbp)	#, %sfp
	je	.L194	#,
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	-1552(%rbp), %rsi	# %sfp,
	movq	-1536(%rbp), %rdi	# %sfp,
	call	_ZdlPvm@PLT	#
.L194:
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	-1328(%rbp), %r14	# MEM[(struct vector *)_1257].D.280086._M_impl.D.279431._M_finish, _701
	movq	-1336(%rbp), %r13	# MEM[(struct vector *)_1257].D.280086._M_impl.D.279431._M_start, __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r13, %r14	# __first, _701
	je	.L200	#,
	.p2align 4,,10
	.p2align 3
.L209:
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	8(%r13), %rbx	# MEM[(struct basic_string * *)__first_1061 + 8B], _736
	movq	0(%r13), %r12	# MEM[(struct basic_string * *)__first_1061], __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r12, %rbx	# __first, _736
	je	.L201	#,
	.p2align 4,,10
	.p2align 3
.L205:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	(%r12), %rdi	# MEM[(char * *)__first_1017], _743
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	16(%r12), %rax	#, tmp859
	cmpq	%rax, %rdi	# tmp859, _743
	je	.L202	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%r12), %rax	# MEM <size_type> [(union ._anon_79 *)__first_1017 + 16B], tmp1161
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$32, %r12	#, __first
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	leaq	1(%rax), %rsi	#, tmp860
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%rbx, %r12	# _736, __first
	jne	.L205	#,
.L204:
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	0(%r13), %r12	# MEM[(struct basic_string * *)__first_1061], __first
.L201:
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%r12, %r12	# __first
	je	.L206	#,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	16(%r13), %rsi	# MEM[(struct basic_string * *)__first_1061 + 16B], tmp862
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r12, %rdi	# __first,
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$24, %r13	#, __first
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%r12, %rsi	# __first, tmp862
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r13, %r14	# __first, _701
	jne	.L209	#,
.L208:
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	-1336(%rbp), %r13	# MEM[(struct _Vector_base *)_1257]._M_impl.D.279431._M_start, __first
.L200:
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%r13, %r13	# __first
	je	.L210	#,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	-1320(%rbp), %rsi	# MEM[(struct _Vector_base *)_1257]._M_impl.D.279431._M_end_of_storage, tmp864
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r13, %rdi	# __first,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%r13, %rsi	# __first, tmp864
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L210:
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	-1352(%rbp), %r14	# MEM[(struct vector *)_1257].D.279045._M_impl.D.278390._M_finish, _693
	movq	-1360(%rbp), %r13	# MEM[(struct vector *)_1257].D.279045._M_impl.D.278390._M_start, __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r13, %r14	# __first, _693
	je	.L211	#,
	.p2align 4,,10
	.p2align 3
.L221:
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	32(%r13), %rbx	# MEM[(struct basic_string * *)__first_1053 + 32B], _753
	movq	24(%r13), %r12	# MEM[(struct basic_string * *)__first_1053 + 24B], __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r12, %rbx	# __first, _753
	je	.L212	#,
	.p2align 4,,10
	.p2align 3
.L216:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	(%r12), %rdi	# MEM[(char * *)__first_1029], _760
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	16(%r12), %rax	#, tmp866
	cmpq	%rax, %rdi	# tmp866, _760
	je	.L213	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%r12), %rax	# MEM <size_type> [(union ._anon_79 *)__first_1029 + 16B], tmp1164
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$32, %r12	#, __first
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	leaq	1(%rax), %rsi	#, tmp867
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%rbx, %r12	# _753, __first
	jne	.L216	#,
.L215:
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	24(%r13), %r12	# MEM[(struct basic_string * *)__first_1053 + 24B], __first
.L212:
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%r12, %r12	# __first
	je	.L217	#,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	40(%r13), %rsi	# MEM[(struct basic_string * *)__first_1053 + 40B], tmp869
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r12, %rdi	# __first,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%r12, %rsi	# __first, tmp869
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L217:
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	0(%r13), %rdi	# MEM[(short unsigned int * *)__first_1053], _749
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%rdi, %rdi	# _749
	je	.L218	#,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	16(%r13), %rsi	# MEM[(short unsigned int * *)__first_1053 + 16B], tmp871
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$48, %r13	#, __first
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%rdi, %rsi	# _749, tmp871
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r14, %r13	# _693, __first
	jne	.L221	#,
.L220:
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	-1360(%rbp), %r13	# MEM[(struct _Vector_base *)_1257]._M_impl.D.278390._M_start, __first
.L211:
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%r13, %r13	# __first
	je	.L222	#,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	-1344(%rbp), %rsi	# MEM[(struct _Vector_base *)_1257]._M_impl.D.278390._M_end_of_storage, tmp873
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r13, %rdi	# __first,
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%r13, %rsi	# __first, tmp873
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L222:
	movq	-1520(%rbp), %r12	# %sfp, tmp926
	leaq	-608(%rbp), %rbx	#, ivtmp.513
	jmp	.L226	#
	.p2align 4,,10
	.p2align 3
.L347:
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%rbx), %rax	# MEM <size_type> [(union ._anon_79 *)_145 + 16B], tmp1168
	leaq	1(%rax), %rsi	#, tmp877
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# tmp_cmp.cpp:185: }
	leaq	-32(%rbx), %rax	#, ivtmp.513
	cmpq	%rbx, %r12	# ivtmp.513, tmp926
	je	.L113	#,
.L225:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	movq	%rax, %rbx	# ivtmp.513, ivtmp.513
.L226:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	(%rbx), %rdi	# MEM[(char * *)_145], _259
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	16(%rbx), %rax	#, tmp876
	cmpq	%rax, %rdi	# tmp876, _259
	jne	.L347	#,
# tmp_cmp.cpp:185: }
	leaq	-32(%rbx), %rax	#, ivtmp.513
	cmpq	%rbx, %r12	# ivtmp.513, tmp926
	jne	.L225	#,
.L113:
	movq	-56(%rbp), %rax	# D.338014, tmp982
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp982
	jne	.L348	#,
	addq	$1528, %rsp	#,
	popq	%rbx	#
	popq	%r12	#
	popq	%r13	#
	popq	%r14	#
	popq	%r15	#
	popq	%rbp	#
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret	
	.p2align 4,,10
	.p2align 3
.L202:
	.cfi_restore_state
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$32, %r12	#, __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r12, %rbx	# __first, _736
	jne	.L205	#,
	jmp	.L204	#
	.p2align 4,,10
	.p2align 3
.L213:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$32, %r12	#, __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r12, %rbx	# __first, _753
	jne	.L216	#,
	jmp	.L215	#
.L123:
# /usr/include/c++/11/bits/locale_facets.h:879: 	this->_M_widen_init();
	movq	%r12, %rdi	# _566,
.LEHB20:
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT	#
# /usr/include/c++/11/bits/locale_facets.h:880: 	return this->do_widen(__c);
	movq	(%r12), %rax	# MEM[(const struct ctype *)_566].D.37784._vptr.facet, MEM[(const struct ctype *)_566].D.37784._vptr.facet
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rcx	#, tmp1050
	movl	$10, %edx	#, _470
	movq	48(%rax), %rax	# MEM[(int (*) () *)_576 + 48B], _577
	cmpq	%rcx, %rax	# tmp1050, _577
	je	.L124	#,
	movl	$10, %esi	#,
	movq	%r12, %rdi	# _566,
	call	*%rax	# _577
.LEHE20:
# /usr/include/c++/11/bits/basic_string.h:6573:     { return std::getline(__is, __str, __is.widen('\n')); }
	movsbl	%al, %edx	# tmp959, _470
	jmp	.L124	#
.L218:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$48, %r13	#, __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r13, %r14	# __first, _693
	jne	.L221	#,
	jmp	.L220	#
.L206:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$24, %r13	#, __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r13, %r14	# __first, _701
	jne	.L209	#,
	jmp	.L208	#
.L197:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$80, %rbx	#, __first
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%rbx, -1480(%rbp)	# __first, %sfp
	jne	.L192	#,
	jmp	.L199	#
.L170:
# tmp_cmp.cpp:124: 	while (!file.eof()) {
	testb	$2, -288(%rbp)	#, MEM[(const struct basic_ios *)&file + 256B].D.41295._M_streambuf_state
	je	.L115	#,
	jmp	.L172	#
.L342:
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	leaq	-1208(%rbp), %rax	#, tmp938
# tmp_cmp.cpp:133: 			size_t cmd_split = line.find(' ');
	xorl	%edx, %edx	#
	movl	$32, %esi	#,
	movq	%r13, %rdi	# tmp930,
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%rax, -1496(%rbp)	# tmp938, %sfp
	movq	%rax, -1224(%rbp)	# tmp938, MEM[(struct _Alloc_hider *)_1253]._M_p
	leaq	-1176(%rbp), %rax	#, tmp939
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movb	$0, -1232(%rbp)	#, MEM[(struct ListLanguageCommand *)_1253].id
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$0, -1216(%rbp)	#, MEM[(struct basic_string *)_1253]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -1208(%rbp)	#, MEM[(char_type &)_1253]
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%rax, -1504(%rbp)	# tmp939, %sfp
	movq	%rax, -1192(%rbp)	# tmp939, MEM[(struct _Alloc_hider *)_1253]._M_p
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$0, -1184(%rbp)	#, MEM[(struct basic_string *)_1253]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, -1176(%rbp)	#, MEM[(char_type &)_1253]
# tmp_cmp.cpp:133: 			size_t cmd_split = line.find(' ');
	call	_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE4findEcm@PLT	#
	movq	%rax, -1472(%rbp)	# tmp961, %sfp
# tmp_cmp.cpp:134: 			while (llc.id<LIST_LANGUAGE_COMMAND_COUNT&&line.substr(1,cmd_split-1)!=mlcmd[llc.id])
	movzbl	-1232(%rbp), %eax	# MEM[(struct ListLanguageCommand *)_1253].id, _6
# tmp_cmp.cpp:134: 			while (llc.id<LIST_LANGUAGE_COMMAND_COUNT&&line.substr(1,cmd_split-1)!=mlcmd[llc.id])
	cmpb	$13, %al	#, _6
	jbe	.L349	#,
	.p2align 4,,10
	.p2align 3
.L137:
# tmp_cmp.cpp:138: 			llc.tail = line.erase(0,cmd_split+1);
	movq	-1472(%rbp), %rsi	# %sfp, cmd_split
# /usr/include/c++/11/bits/basic_string.h:921:       { return _M_string_length; }
	movq	-1288(%rbp), %rdx	# line._M_string_length, _610
# tmp_cmp.cpp:138: 			llc.tail = line.erase(0,cmd_split+1);
	leaq	1(%rsi), %rax	#, _11
# /usr/include/c++/11/bits/basic_string.h:1830: 	if (__n == npos)
	cmpq	$-2, %rsi	#, cmd_split
	je	.L350	#,
# /usr/include/c++/11/bits/basic_string.h:1832: 	else if (__n != 0)
	testq	%rax, %rax	# _11
	jne	.L351	#,
.L140:
# /usr/include/c++/11/bits/basic_string.h:1387: 	this->_M_assign(__str);
	leaq	-1224(%rbp), %rdi	#, tmp664
	movq	%r13, %rsi	# tmp930,
	leaq	-1232(%rbp), %r14	#, tmp935
.LEHB21:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_@PLT	#
.LEHE21:
# tmp_cmp.cpp:139: 			llc.line_number = line_number;
	movzwl	-1440(%rbp), %eax	# %sfp, line_number
# /usr/include/c++/11/bits/stl_vector.h:1189: 	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
	movq	-1480(%rbp), %rcx	# %sfp, prephitmp_466
# tmp_cmp.cpp:139: 			llc.line_number = line_number;
	movw	%ax, -1160(%rbp)	# line_number, MEM[(struct ListLanguageCommand *)_1253].line_number
# /usr/include/c++/11/bits/stl_vector.h:1189: 	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
	movq	-1488(%rbp), %rax	# %sfp, prephitmp_469
	cmpq	%rax, %rcx	# prephitmp_469, prephitmp_466
	je	.L142	#,
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movzbl	-1232(%rbp), %eax	# MEM[(const struct ListLanguageCommand &)_1253].id, _619
# /usr/include/c++/11/bits/basic_string.h:201: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	leaq	24(%rcx), %r15	#, _621
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%r15, 8(%rcx)	# _621, MEM[(struct _Alloc_hider *)prephitmp_466 + 8B]._M_p
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movb	%al, (%rcx)	# _619, *prephitmp_466.id
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1224(%rbp), %r13	# MEM[(const struct basic_string *)_1253]._M_dataplus._M_p, _622
# /usr/include/c++/11/bits/basic_string.h:927:       { return _M_string_length; }
	movq	-1216(%rbp), %r12	# MEM[(const struct basic_string *)_1253]._M_string_length, prephitmp_531
# /usr/include/c++/11/bits/basic_string.tcc:211: 	if (__gnu_cxx::__is_null_pointer(__beg) && __beg != __end)
	movq	%r13, %rax	# _622, tmp979
	addq	%r12, %rax	# prephitmp_531, tmp979
	je	.L143	#,
	testq	%r13, %r13	# _622
	je	.L352	#,
.L143:
# /usr/include/c++/11/bits/basic_string.tcc:215: 	size_type __dnew = static_cast<size_type>(std::distance(__beg, __end));
	movq	%r12, -1416(%rbp)	# prephitmp_531, MEM[(long unsigned int *)_1255]
# /usr/include/c++/11/bits/basic_string.tcc:217: 	if (__dnew > size_type(_S_local_capacity))
	cmpq	$15, %r12	#, prephitmp_531
	ja	.L353	#,
# /usr/include/c++/11/bits/basic_string.h:356: 	if (__n == 1)
	cmpq	$1, %r12	#, prephitmp_531
	jne	.L146	#,
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movzbl	0(%r13), %eax	# MEM[(const char_type &)_622], _639
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movq	-1480(%rbp), %rcx	# %sfp, prephitmp_466
	movb	%al, 24(%rcx)	# _639, MEM[(char_type &)prephitmp_466 + 24]
# /usr/include/c++/11/bits/basic_string.h:201: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	movq	%r15, %rax	# _621, pretmp_541
.L147:
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	-1480(%rbp), %rcx	# %sfp, prephitmp_466
# /usr/include/c++/11/bits/basic_string.h:201: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	leaq	56(%rcx), %rdi	#, _626
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	%r12, 16(%rcx)	# prephitmp_531, MEM[(struct basic_string *)prephitmp_466 + 8B]._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%rax,%r12)	#, MEM[(char_type &)_642]
# /usr/include/c++/11/bits/basic_string.h:927:       { return _M_string_length; }
	movq	-1184(%rbp), %r12	# MEM[(const struct basic_string *)_1253]._M_string_length, prephitmp_547
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	movq	%rdi, 40(%rcx)	# _626, MEM[(struct _Alloc_hider *)prephitmp_466 + 40B]._M_p
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1192(%rbp), %r13	# MEM[(const struct basic_string *)_1253]._M_dataplus._M_p, _627
# /usr/include/c++/11/bits/basic_string.tcc:211: 	if (__gnu_cxx::__is_null_pointer(__beg) && __beg != __end)
	movq	%r13, %rax	# _627, tmp980
	addq	%r12, %rax	# prephitmp_547, tmp980
	je	.L148	#,
	testq	%r13, %r13	# _627
	je	.L354	#,
.L148:
# /usr/include/c++/11/bits/basic_string.tcc:215: 	size_type __dnew = static_cast<size_type>(std::distance(__beg, __end));
	movq	%r12, -1416(%rbp)	# prephitmp_547, MEM[(long unsigned int *)_1255]
# /usr/include/c++/11/bits/basic_string.tcc:217: 	if (__dnew > size_type(_S_local_capacity))
	cmpq	$15, %r12	#, prephitmp_547
	ja	.L355	#,
# /usr/include/c++/11/bits/basic_string.h:356: 	if (__n == 1)
	cmpq	$1, %r12	#, prephitmp_547
	jne	.L151	#,
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movzbl	0(%r13), %eax	# MEM[(const char_type &)_627], _648
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movq	-1480(%rbp), %rcx	# %sfp, prephitmp_466
	movb	%al, 56(%rcx)	# _648, MEM[(char_type &)prephitmp_466 + 56]
.L152:
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	-1480(%rbp), %rcx	# %sfp, prephitmp_466
	movq	%r12, 48(%rcx)	# prephitmp_547, MEM[(struct basic_string *)prephitmp_466 + 40B]._M_string_length
# /usr/include/c++/11/bits/stl_vector.h:1194: 	    ++this->_M_impl._M_finish;
	addq	$80, %rcx	#, prephitmp_466
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%rdi,%r12)	#, MEM[(char_type &)_651]
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movzwl	-1160(%rbp), %eax	# MEM[(const struct ListLanguageCommand &)_1253].line_number, _630
	movw	%ax, -8(%rcx)	# _630, *prephitmp_466.line_number
# /usr/include/c++/11/bits/stl_vector.h:1194: 	    ++this->_M_impl._M_finish;
	movq	%rcx, -1480(%rbp)	# prephitmp_466, %sfp
	movq	%rcx, -1400(%rbp)	# prephitmp_466, cmd_buffer.D.296089._M_impl.D.295434._M_finish
.L153:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1192(%rbp), %rdi	# MEM[(const struct basic_string *)_1253]._M_dataplus._M_p, _652
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	-1504(%rbp), %rdi	# %sfp, _652
	je	.L157	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	-1176(%rbp), %rax	# MEM[(struct basic_string *)_1253].D.25367._M_allocated_capacity, tmp1099
	leaq	1(%rax), %rsi	#, tmp705
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L157:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1224(%rbp), %rdi	# MEM[(const struct basic_string *)_1253]._M_dataplus._M_p, _656
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	-1496(%rbp), %rdi	# %sfp, _656
	je	.L159	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	-1208(%rbp), %rax	# MEM[(struct basic_string *)_1253].D.25367._M_allocated_capacity, tmp1101
	leaq	1(%rax), %rsi	#, tmp709
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	jmp	.L159	#
.L163:
# /usr/include/c++/11/bits/char_traits.h:431: 	if (__n == 0)
	testq	%r12, %r12	# prephitmp_482
	jne	.L356	#,
	movq	%rbx, %rax	# tmp936, pretmp_483
	leaq	-1232(%rbp), %r14	#, tmp935
	jmp	.L164	#
.L344:
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	leaq	-1232(%rbp), %r14	#, tmp935
	leaq	-1416(%rbp), %rsi	#, tmp719
	xorl	%edx, %edx	#
	movq	%r14, %rdi	# tmp935,
.LEHB22:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT	#
.LEHE22:
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, -1232(%rbp)	# _42, MEM[(struct basic_string *)_1253]._M_dataplus._M_p
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	movq	%rax, %rdi	# tmp967, _42
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	-1416(%rbp), %rax	# MEM[(long unsigned int *)_1255], MEM[(long unsigned int *)_1255]
	movq	%rax, -1216(%rbp)	# MEM[(long unsigned int *)_1255], MEM[(struct basic_string *)_1253].D.25367._M_allocated_capacity
.L162:
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	%r12, %rdx	# prephitmp_482,
	movq	%r13, %rsi	# _662,
	call	memcpy@PLT	#
# /usr/include/c++/11/bits/basic_string.tcc:232: 	_M_set_length(__dnew);
	movq	-1416(%rbp), %r12	# MEM[(long unsigned int *)_1255], prephitmp_482
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1232(%rbp), %rax	# MEM[(struct basic_string *)_1253]._M_dataplus._M_p, pretmp_483
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	jmp	.L164	#
.L350:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1296(%rbp), %rax	# line._M_dataplus._M_p, _611
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	$0, -1288(%rbp)	#, line._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%rax)	#, MEM[(char_type &)_611]
# /usr/include/c++/11/bits/basic_string.h:226:       }
	jmp	.L140	#
.L349:
# tmp_cmp.cpp:134: 			while (llc.id<LIST_LANGUAGE_COMMAND_COUNT&&line.substr(1,cmd_split-1)!=mlcmd[llc.id])
	movq	-1472(%rbp), %rcx	# %sfp, cmd_split
	subq	$1, %rcx	#, _492
	movq	%rcx, -1464(%rbp)	# _492, %sfp
	.p2align 4,,10
	.p2align 3
.L127:
# tmp_cmp.cpp:134: 			while (llc.id<LIST_LANGUAGE_COMMAND_COUNT&&line.substr(1,cmd_split-1)!=mlcmd[llc.id])
	movq	-1520(%rbp), %rsi	# %sfp, tmp926
	movzbl	%al, %eax	# _6, _6
# /usr/include/c++/11/bits/basic_string.h:921:       { return _M_string_length; }
	movq	-1288(%rbp), %r12	# line._M_string_length, _587
# tmp_cmp.cpp:134: 			while (llc.id<LIST_LANGUAGE_COMMAND_COUNT&&line.substr(1,cmd_split-1)!=mlcmd[llc.id])
	salq	$5, %rax	#, tmp636
	leaq	(%rsi,%rax), %r15	#, _9
# /usr/include/c++/11/bits/basic_string.h:320: 	if (__pos > this->size())
	testq	%r12, %r12	# _587
	je	.L357	#,
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1296(%rbp), %rax	# MEM[(const struct basic_string *)&line]._M_dataplus._M_p, _589
# /usr/include/c++/11/bits/basic_string.h:339: 	const bool __testoff =  __off < this->size() - __pos;
	subq	$1, %r12	#, tmp641
# /usr/include/c++/11/bits/basic_string.h:168: 	: allocator_type(std::move(__a)), _M_p(__dat) { }
	leaq	-1248(%rbp), %rbx	#, tmp934
	movq	%rbx, -1264(%rbp)	# tmp934, MEM[(struct _Alloc_hider *)&D.296290]._M_p
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	%rax, -1448(%rbp)	# _589, %sfp
	movq	-1464(%rbp), %rax	# %sfp, _492
	cmpq	%rax, %r12	# _492, tmp641
	cmova	%rax, %r12	# tmp641,, _492, prephitmp_506
# /usr/include/c++/11/bits/basic_string.tcc:215: 	size_type __dnew = static_cast<size_type>(std::distance(__beg, __end));
	movq	%r12, -1416(%rbp)	# prephitmp_506, MEM[(long unsigned int *)_1255]
# /usr/include/c++/11/bits/basic_string.tcc:217: 	if (__dnew > size_type(_S_local_capacity))
	cmpq	$15, %r12	#, prephitmp_506
	ja	.L358	#,
# /usr/include/c++/11/bits/basic_string.h:356: 	if (__n == 1)
	cmpq	$1, %r12	#, prephitmp_506
	jne	.L131	#,
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movq	-1448(%rbp), %rax	# %sfp, _589
	movzbl	1(%rax), %eax	# MEM[(const char_type &)_589 + 1], _600
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	%al, -1248(%rbp)	# _600, MEM[(char_type &)&D.296290 + 16]
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movq	%rbx, %rax	# tmp934, pretmp_508
.L132:
# /usr/include/c++/11/bits/basic_string.h:191:       { _M_string_length = __length; }
	movq	%r12, -1256(%rbp)	# prephitmp_506, D.296290._M_string_length
# /usr/include/c++/11/bits/char_traits.h:357:       { __c1 = __c2; }
	movb	$0, (%rax,%r12)	#, MEM[(char_type &)_603]
	movq	-1264(%rbp), %r12	# MEM[(char * *)&D.296290], _344
	movq	-1256(%rbp), %rdx	# MEM[(long unsigned int *)&D.296290 + 8B], _345
	movq	(%r15), %rsi	# MEM[(char * *)_9], _346
# /usr/include/c++/11/bits/basic_string.h:6237: 	      && !std::char_traits<_CharT>::compare(__lhs.data(), __rhs.data(),
	cmpq	8(%r15), %rdx	# MEM[(long unsigned int *)_9 + 8B], _345
	je	.L359	#,
.L133:
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	%rbx, %r12	# tmp934, _344
	jne	.L360	#,
.L333:
# tmp_cmp.cpp:135: 				llc.id++;
	movzbl	-1232(%rbp), %eax	# MEM[(struct ListLanguageCommand *)_1253].id, tmp1057
	addl	$1, %eax	#, _6
	movb	%al, -1232(%rbp)	# _6, MEM[(struct ListLanguageCommand *)_1253].id
# tmp_cmp.cpp:134: 			while (llc.id<LIST_LANGUAGE_COMMAND_COUNT&&line.substr(1,cmd_split-1)!=mlcmd[llc.id])
	cmpb	$13, %al	#, _6
	ja	.L137	#,
	jmp	.L127	#
.L131:
# /usr/include/c++/11/bits/char_traits.h:431: 	if (__n == 0)
	testq	%r12, %r12	# prephitmp_506
	jne	.L361	#,
	movq	%rbx, %rax	# tmp934, pretmp_508
	jmp	.L132	#
.L359:
# /usr/include/c++/11/bits/char_traits.h:374: 	if (__n == 0)
	testq	%rdx, %rdx	# _345
	je	.L134	#,
# /usr/include/c++/11/bits/char_traits.h:389: 	return __builtin_memcmp(__s1, __s2, __n);
	movq	%r12, %rdi	# _344,
	call	memcmp@PLT	#
# /usr/include/c++/11/bits/basic_string.h:6237: 	      && !std::char_traits<_CharT>::compare(__lhs.data(), __rhs.data(),
	testl	%eax, %eax	# tmp963
	jne	.L133	#,
.L134:
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	%rbx, %r12	# tmp934, _344
	je	.L137	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	-1248(%rbp), %rax	# D.296290.D.25367._M_allocated_capacity, tmp1066
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r12, %rdi	# _344,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	leaq	1(%rax), %rsi	#, tmp658
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
	jmp	.L137	#
.L358:
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	movq	-1512(%rbp), %rdi	# %sfp,
	leaq	-1416(%rbp), %rsi	#, tmp642
	xorl	%edx, %edx	#
	leaq	-1232(%rbp), %r14	#, tmp935
.LEHB23:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT	#
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, -1264(%rbp)	# _369, D.296290._M_dataplus._M_p
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	movq	%rax, %rdi	# tmp962, _369
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	-1416(%rbp), %rax	# MEM[(long unsigned int *)_1255], MEM[(long unsigned int *)_1255]
	movq	%rax, -1248(%rbp)	# MEM[(long unsigned int *)_1255], D.296290.D.25367._M_allocated_capacity
.L130:
# /usr/include/c++/11/bits/basic_string.h:488: 	const _CharT* __start = __str._M_data()
	movq	-1448(%rbp), %rsi	# %sfp, _589
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	%r12, %rdx	# prephitmp_506,
# /usr/include/c++/11/bits/basic_string.h:488: 	const _CharT* __start = __str._M_data()
	addq	$1, %rsi	#, _589
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	call	memcpy@PLT	#
# /usr/include/c++/11/bits/basic_string.tcc:232: 	_M_set_length(__dnew);
	movq	-1416(%rbp), %r12	# MEM[(long unsigned int *)_1255], prephitmp_506
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1264(%rbp), %rax	# D.296290._M_dataplus._M_p, pretmp_508
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	jmp	.L132	#
.L142:
# /usr/include/c++/11/bits/stl_vector.h:1198: 	  _M_realloc_insert(end(), __x);
	leaq	-1232(%rbp), %r14	#, tmp935
	movq	%rcx, %rsi	# prephitmp_466,
	leaq	-1408(%rbp), %rdi	#, tmp702
	movq	%r14, %rdx	# tmp935,
	call	_ZNSt6vectorI19ListLanguageCommandSaIS0_EE17_M_realloc_insertIJRKS0_EEEvN9__gnu_cxx17__normal_iteratorIPS0_S2_EEDpOT_	#
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	-1392(%rbp), %rax	# MEM[(struct _Vector_base *)&cmd_buffer]._M_impl.D.295434._M_end_of_storage, prephitmp_469
	movq	%rax, -1488(%rbp)	# prephitmp_469, %sfp
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	-1400(%rbp), %rax	# cmd_buffer.D.296089._M_impl.D.295434._M_finish, prephitmp_466
	movq	%rax, -1480(%rbp)	# prephitmp_466, %sfp
	jmp	.L153	#
.L151:
# /usr/include/c++/11/bits/char_traits.h:431: 	if (__n == 0)
	testq	%r12, %r12	# prephitmp_547
	je	.L152	#,
.L150:
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	%r12, %rdx	# prephitmp_547,
	movq	%r13, %rsi	# _627,
	call	memcpy@PLT	#
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1480(%rbp), %rax	# %sfp, prephitmp_466
# /usr/include/c++/11/bits/basic_string.tcc:232: 	_M_set_length(__dnew);
	movq	-1416(%rbp), %r12	# MEM[(long unsigned int *)_1255], prephitmp_547
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	40(%rax), %rdi	# MEM[(const struct basic_string *)prephitmp_466 + 40B]._M_dataplus._M_p, _626
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	jmp	.L152	#
.L146:
# /usr/include/c++/11/bits/char_traits.h:431: 	if (__n == 0)
	testq	%r12, %r12	# prephitmp_531
	jne	.L362	#,
# /usr/include/c++/11/bits/basic_string.h:201: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	movq	%r15, %rax	# _621, pretmp_541
	jmp	.L147	#
.L351:
# /usr/include/c++/11/bits/basic_string.h:1833: 	  this->_M_erase(__pos, _M_limit(__pos, __n));
	cmpq	%rdx, %rax	# _610, _11
	movq	%r13, %rdi	# tmp930,
	leaq	-1232(%rbp), %r14	#, tmp935
	cmovbe	%rax, %rdx	# _11,, tmp660
	xorl	%esi, %esi	#
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE8_M_eraseEmm@PLT	#
.LEHE23:
	jmp	.L140	#
.L355:
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movq	-1480(%rbp), %rbx	# %sfp, prephitmp_466
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	leaq	-1416(%rbp), %rsi	#, tmp687
	xorl	%edx, %edx	#
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	leaq	40(%rbx), %rdi	#, tmp688
.LEHB24:
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT	#
.LEHE24:
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, 40(%rbx)	# _626, MEM[(struct basic_string *)prephitmp_466 + 40B]._M_dataplus._M_p
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	movq	%rax, %rdi	# tmp965, _626
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	-1416(%rbp), %rax	# MEM[(long unsigned int *)_1255], MEM[(long unsigned int *)_1255]
	movq	%rax, 56(%rbx)	# MEM[(long unsigned int *)_1255], MEM[(struct basic_string *)prephitmp_466 + 40B].D.25367._M_allocated_capacity
	jmp	.L150	#
.L353:
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	movq	-1480(%rbp), %rbx	# %sfp, prephitmp_466
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	leaq	-1416(%rbp), %rsi	#, tmp672
	xorl	%edx, %edx	#
	leaq	-1232(%rbp), %r14	#, tmp935
# ./script/menu/main_menu.h:147: struct ListLanguageCommand
	leaq	8(%rbx), %rdi	#, tmp673
.LEHB25:
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm@PLT	#
.LEHE25:
# /usr/include/c++/11/bits/basic_string.h:187:       { _M_dataplus._M_p = __p; }
	movq	%rax, 8(%rbx)	# _636, MEM[(struct basic_string *)prephitmp_466 + 8B]._M_dataplus._M_p
# /usr/include/c++/11/bits/basic_string.tcc:219: 	    _M_data(_M_create(__dnew, size_type(0)));
	movq	%rax, %rdi	# tmp964, _636
# /usr/include/c++/11/bits/basic_string.h:219:       { _M_allocated_capacity = __capacity; }
	movq	-1416(%rbp), %rax	# MEM[(long unsigned int *)_1255], MEM[(long unsigned int *)_1255]
	movq	%rax, 24(%rbx)	# MEM[(long unsigned int *)_1255], MEM[(struct basic_string *)prephitmp_466 + 8B].D.25367._M_allocated_capacity
.L145:
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	%r12, %rdx	# prephitmp_531,
	movq	%r13, %rsi	# _622,
	call	memcpy@PLT	#
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1480(%rbp), %rax	# %sfp, prephitmp_466
# /usr/include/c++/11/bits/basic_string.tcc:232: 	_M_set_length(__dnew);
	movq	-1416(%rbp), %r12	# MEM[(long unsigned int *)_1255], prephitmp_531
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	8(%rax), %rax	# MEM[(const struct basic_string *)prephitmp_466 + 8B]._M_dataplus._M_p, pretmp_541
# /usr/include/c++/11/bits/char_traits.h:437: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	jmp	.L147	#
.L339:
# /usr/include/c++/11/bits/ios_base.h:170:   { return _Ios_Iostate(static_cast<int>(__a) | static_cast<int>(__b)); }
	movl	32(%rbx), %esi	# MEM[(const struct basic_ios *)_203].D.41295._M_streambuf_state, tmp601
	orl	$4, %esi	#, tmp601
.LEHB26:
# /usr/include/c++/11/bits/basic_ios.h:158:       { this->clear(this->rdstate() | __state); }
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT	#
.LEHE26:
# tmp_cmp.cpp:124: 	while (!file.eof()) {
	testb	$2, -288(%rbp)	#, MEM[(const struct basic_ios *)&file + 256B].D.41295._M_streambuf_state
	je	.L251	#,
.L340:
	leaq	-1024(%rbp), %rax	#, tmp926
	movq	$0, -1552(%rbp)	#, %sfp
	movq	$0, -1480(%rbp)	#, %sfp
	movq	$0, -1536(%rbp)	#, %sfp
	movq	%rax, -1520(%rbp)	# tmp926, %sfp
	jmp	.L116	#
.L345:
# /usr/include/c++/11/fstream:666: 	  this->setstate(ios_base::failbit);
	movq	-576(%rbp), %rax	# file.D.49057._vptr.basic_istream, file.D.49057._vptr.basic_istream
	movq	-1528(%rbp), %rdi	# %sfp, _684
	addq	-24(%rax), %rdi	# MEM[(long int *)_681 + -24B], _684
# /usr/include/c++/11/bits/ios_base.h:170:   { return _Ios_Iostate(static_cast<int>(__a) | static_cast<int>(__b)); }
	movl	32(%rdi), %esi	# MEM[(const struct basic_ios *)_684].D.41295._M_streambuf_state, tmp758
	orl	$4, %esi	#, tmp758
.LEHB27:
# /usr/include/c++/11/bits/basic_ios.h:158:       { this->clear(this->rdstate() | __state); }
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT	#
.LEHE27:
# /usr/include/c++/11/bits/basic_ios.h:158:       { this->clear(this->rdstate() | __state); }
	jmp	.L173	#
.L361:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	%rbx, %rdi	# tmp934, _369
	jmp	.L130	#
.L354:
# /usr/include/c++/11/bits/basic_string.tcc:212: 	  std::__throw_logic_error(__N("basic_string::"
	leaq	.LC20(%rip), %rdi	#, tmp686
.LEHB28:
	call	_ZSt19__throw_logic_errorPKc@PLT	#
.LEHE28:
.L348:
# tmp_cmp.cpp:185: }
	call	__stack_chk_fail@PLT	#
.L362:
# /usr/include/c++/11/bits/basic_string.h:201: 	return std::pointer_traits<pointer>::pointer_to(*_M_local_buf);
	movq	%r15, %rdi	# _621, _636
	jmp	.L145	#
	.p2align 4,,10
	.p2align 3
.L357:
# /usr/include/c++/11/bits/basic_string.h:321: 	  __throw_out_of_range_fmt(__N("%s: __pos (which is %zu) > "
	xorl	%ecx, %ecx	#
	movl	$1, %edx	#,
	leaq	.LC21(%rip), %rsi	#, tmp637
	xorl	%eax, %eax	#
	leaq	.LC22(%rip), %rdi	#, tmp638
	leaq	-1232(%rbp), %r14	#, tmp935
.LEHB29:
	call	_ZSt24__throw_out_of_range_fmtPKcz@PLT	#
.LEHE29:
.L341:
.LEHB30:
# /usr/include/c++/11/bits/basic_ios.h:50: 	__throw_bad_cast();
	call	_ZSt16__throw_bad_castv@PLT	#
.L343:
# /usr/include/c++/11/bits/basic_string.tcc:212: 	  std::__throw_logic_error(__N("basic_string::"
	leaq	.LC20(%rip), %rdi	#, tmp718
	call	_ZSt19__throw_logic_errorPKc@PLT	#
.LEHE30:
.L352:
	leaq	.LC20(%rip), %rdi	#, tmp671
.LEHB31:
	call	_ZSt19__throw_logic_errorPKc@PLT	#
.LEHE31:
.L356:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	%rbx, %rdi	# tmp936, _42
	leaq	-1232(%rbp), %r14	#, tmp935
	jmp	.L162	#
.L360:
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	-1248(%rbp), %rax	# D.296290.D.25367._M_allocated_capacity, tmp1064
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r12, %rdi	# _344,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	leaq	1(%rax), %rsi	#, tmp655
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
	jmp	.L333	#
.L268:
	endbr64	
# /usr/include/c++/11/fstream:255: 	__catch(...)
	movq	%rax, %rdi	# tmp972, tmp852
	jmp	.L195	#
.L263:
	endbr64	
# /usr/include/c++/11/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	%rax, %r12	# tmp958, tmp612
	jmp	.L120	#
.L266:
	endbr64	
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	%rax, %rbx	# tmp966, tmp698
	jmp	.L154	#
.L258:
	endbr64	
# tmp_cmp.cpp:105: 	};
	movq	%rax, %r12	# tmp973, tmp883
	jmp	.L227	#
.L262:
	endbr64	
# tmp_cmp.cpp:185: }
	movq	%rax, %rbx	# tmp977, tmp895
	jmp	.L233	#
.L259:
	endbr64	
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	%rax, %rbx	# tmp976, tmp733
	jmp	.L168	#
.L261:
	endbr64	
	movq	%rax, %r12	# tmp975, tmp889
	jmp	.L230	#
.L267:
	endbr64	
	movq	%rax, %r12	# tmp968, tmp734
	jmp	.L230	#
.L260:
	endbr64	
# tmp_cmp.cpp:143: 		} else cmd_buffer.back().buffer += line+' ';
	movq	%rax, %rbx	# tmp974, tmp697
	jmp	.L156	#
.L265:
	endbr64	
# /usr/include/c++/11/fstream:539:       }
	movq	%rax, %r12	# tmp956, tmp608
	jmp	.L118	#
.L264:
	endbr64	
# /usr/include/c++/11/istream:104:       { _M_gcount = streamsize(0); }
	movq	%rax, %r12	# tmp957, tmp607
	jmp	.L119	#
	.section	.gcc_except_table
	.align 4
.LLSDA13443:
	.byte	0xff
	.byte	0x9b
	.uleb128 .LLSDATT13443-.LLSDATTD13443
.LLSDATTD13443:
	.byte	0x1
	.uleb128 .LLSDACSE13443-.LLSDACSB13443
.LLSDACSB13443:
	.uleb128 .LEHB11-.LFB13443
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L258-.LFB13443
	.uleb128 0
	.uleb128 .LEHB12-.LFB13443
	.uleb128 .LEHE12-.LEHB12
	.uleb128 .L263-.LFB13443
	.uleb128 0
	.uleb128 .LEHB13-.LFB13443
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L264-.LFB13443
	.uleb128 0
	.uleb128 .LEHB14-.LFB13443
	.uleb128 .LEHE14-.LEHB14
	.uleb128 .L265-.LFB13443
	.uleb128 0
	.uleb128 .LEHB15-.LFB13443
	.uleb128 .LEHE15-.LEHB15
	.uleb128 .L259-.LFB13443
	.uleb128 0
	.uleb128 .LEHB16-.LFB13443
	.uleb128 .LEHE16-.LEHB16
	.uleb128 .L267-.LFB13443
	.uleb128 0
	.uleb128 .LEHB17-.LFB13443
	.uleb128 .LEHE17-.LEHB17
	.uleb128 .L261-.LFB13443
	.uleb128 0
	.uleb128 .LEHB18-.LFB13443
	.uleb128 .LEHE18-.LEHB18
	.uleb128 .L262-.LFB13443
	.uleb128 0
	.uleb128 .LEHB19-.LFB13443
	.uleb128 .LEHE19-.LEHB19
	.uleb128 .L268-.LFB13443
	.uleb128 0x1
	.uleb128 .LEHB20-.LFB13443
	.uleb128 .LEHE20-.LEHB20
	.uleb128 .L259-.LFB13443
	.uleb128 0
	.uleb128 .LEHB21-.LFB13443
	.uleb128 .LEHE21-.LEHB21
	.uleb128 .L260-.LFB13443
	.uleb128 0
	.uleb128 .LEHB22-.LFB13443
	.uleb128 .LEHE22-.LEHB22
	.uleb128 .L259-.LFB13443
	.uleb128 0
	.uleb128 .LEHB23-.LFB13443
	.uleb128 .LEHE23-.LEHB23
	.uleb128 .L260-.LFB13443
	.uleb128 0
	.uleb128 .LEHB24-.LFB13443
	.uleb128 .LEHE24-.LEHB24
	.uleb128 .L266-.LFB13443
	.uleb128 0
	.uleb128 .LEHB25-.LFB13443
	.uleb128 .LEHE25-.LEHB25
	.uleb128 .L260-.LFB13443
	.uleb128 0
	.uleb128 .LEHB26-.LFB13443
	.uleb128 .LEHE26-.LEHB26
	.uleb128 .L265-.LFB13443
	.uleb128 0
	.uleb128 .LEHB27-.LFB13443
	.uleb128 .LEHE27-.LEHB27
	.uleb128 .L262-.LFB13443
	.uleb128 0
	.uleb128 .LEHB28-.LFB13443
	.uleb128 .LEHE28-.LEHB28
	.uleb128 .L266-.LFB13443
	.uleb128 0
	.uleb128 .LEHB29-.LFB13443
	.uleb128 .LEHE29-.LEHB29
	.uleb128 .L260-.LFB13443
	.uleb128 0
	.uleb128 .LEHB30-.LFB13443
	.uleb128 .LEHE30-.LEHB30
	.uleb128 .L259-.LFB13443
	.uleb128 0
	.uleb128 .LEHB31-.LFB13443
	.uleb128 .LEHE31-.LEHB31
	.uleb128 .L260-.LFB13443
	.uleb128 0
.LLSDACSE13443:
	.byte	0x1
	.byte	0
	.align 4
	.long	0

.LLSDATT13443:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC13443
	.type	_ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE.cold, @function
_ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE.cold:
.LFSB13443:
.L195:
	.cfi_def_cfa 6, 16
	.cfi_offset 3, -56
	.cfi_offset 6, -16
	.cfi_offset 12, -48
	.cfi_offset 13, -40
	.cfi_offset 14, -32
	.cfi_offset 15, -24
# /usr/include/c++/11/fstream:255: 	__catch(...)
	call	__cxa_begin_catch@PLT	#
	call	__cxa_end_catch@PLT	#
	jmp	.L179	#
.L118:
# /usr/include/c++/11/fstream:539:       }
	movq	-1544(%rbp), %rdi	# %sfp,
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEED1Ev@PLT	#
.L119:
# /usr/include/c++/11/istream:104:       { _M_gcount = streamsize(0); }
	movq	8+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rax	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 8B], tmp1041
	movq	16+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rcx	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 16B], tmp1043
	movq	%rax, -576(%rbp)	# tmp1041, MEM[(struct basic_istream *)&file]._vptr.basic_istream
	movq	-24(%rax), %rax	# MEM[(long int *)_532 + -24B], MEM[(long int *)_532 + -24B]
	movq	%rcx, -576(%rbp,%rax)	# tmp1043, MEM[(struct basic_ios *)_545].D.41295._vptr.ios_base
# /usr/include/c++/11/istream:104:       { _M_gcount = streamsize(0); }
	movq	$0, -568(%rbp)	#, MEM[(struct basic_istream *)&file]._M_gcount
.L120:
# /usr/include/c++/11/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	-1560(%rbp), %rdi	# %sfp,
	leaq	16+_ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax	#, tmp1044
	movq	%rax, -320(%rbp)	# tmp1044, MEM[(struct basic_ios *)&file + 256B].D.41295._vptr.ios_base
	call	_ZNSt8ios_baseD2Ev@PLT	#
	leaq	-1024(%rbp), %rax	#, tmp926
	movq	%rax, -1520(%rbp)	# tmp926, %sfp
.L121:
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	-1408(%rbp), %r14	# cmd_buffer.D.296089._M_impl.D.295434._M_start, _766
	movq	-1400(%rbp), %r13	# cmd_buffer.D.296089._M_impl.D.295434._M_finish, _765
	movq	%r14, %rbx	# _766, __first
.L234:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%rbx, %r13	# __first, _765
	je	.L363	#,
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	40(%rbx), %rdi	# MEM[(char * *)__first_767 + 40B], _773
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	56(%rbx), %rax	#, tmp898
	cmpq	%rax, %rdi	# tmp898, _773
	je	.L235	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	56(%rbx), %rax	# MEM <size_type> [(union ._anon_79 *)__first_767 + 56B], tmp1175
	leaq	1(%rax), %rsi	#, tmp899
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L235:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	8(%rbx), %rdi	# MEM[(char * *)__first_767 + 8B], _777
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	24(%rbx), %rax	#, tmp901
	cmpq	%rax, %rdi	# tmp901, _777
	je	.L236	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	24(%rbx), %rax	# MEM <size_type> [(union ._anon_79 *)__first_767 + 24B], tmp1176
	leaq	1(%rax), %rsi	#, tmp902
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L236:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$80, %rbx	#, __first
	jmp	.L234	#
.L154:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1480(%rbp), %rax	# %sfp, prephitmp_466
	movq	8(%rax), %rdi	# MEM[(const struct basic_string *)prephitmp_466 + 8B]._M_dataplus._M_p, _631
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	%rdi, %r15	# _631, _621
	je	.L155	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	24(%rax), %rsi	# MEM[(struct basic_string *)prephitmp_466 + 8B].D.25367._M_allocated_capacity, MEM[(struct basic_string *)prephitmp_466 + 8B].D.25367._M_allocated_capacity
	addq	$1, %rsi	#, tmp695
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L155:
	leaq	-1232(%rbp), %r14	#, tmp935
.L156:
# tmp_cmp.cpp:143: 		} else cmd_buffer.back().buffer += line+' ';
	movq	%r14, %rdi	# tmp935,
	call	_ZN19ListLanguageCommandD1Ev	#
.L168:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1296(%rbp), %rdi	# line._M_dataplus._M_p, _270
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	-1432(%rbp), %rdi	# %sfp, _270
	je	.L233	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	-1280(%rbp), %rax	# line.D.25367._M_allocated_capacity, tmp1174
	leaq	1(%rax), %rsi	#, tmp893
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L233:
# tmp_cmp.cpp:185: }
	movq	-1528(%rbp), %rdi	# %sfp,
	movq	%rbx, %r12	# tmp895, tmp617
	call	_ZNSt14basic_ifstreamIcSt11char_traitsIcEED1Ev@PLT	#
	jmp	.L121	#
.L363:
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	-1392(%rbp), %rsi	# MEM[(struct _Vector_base *)&cmd_buffer]._M_impl.D.295434._M_end_of_storage, _771
	subq	%r14, %rsi	# _766, _771
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%r14, %r14	# _766
	je	.L238	#,
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	movq	%r14, %rdi	# _766,
	call	_ZdlPvm@PLT	#
.L238:
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	-1328(%rbp), %rbx	# MEM[(struct vector *)_1257].D.280086._M_impl.D.279431._M_finish, _781
	movq	-1336(%rbp), %r14	# MEM[(struct vector *)_1257].D.280086._M_impl.D.279431._M_start, __first
.L239:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r14, %rbx	# __first, _781
	je	.L364	#,
# /usr/include/c++/11/bits/stl_vector.h:680: 	std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
	movq	8(%r14), %r15	# MEM[(struct basic_string * *)__first_783 + 8B], _789
	movq	(%r14), %r13	# MEM[(struct basic_string * *)__first_783], __first
.L240:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	cmpq	%r13, %r15	# __first, _789
	je	.L365	#,
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	0(%r13), %rdi	# MEM[(char * *)__first_795], _796
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	16(%r13), %rax	#, tmp907
	cmpq	%rax, %rdi	# tmp907, _796
	je	.L241	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%r13), %rax	# MEM <size_type> [(union ._anon_79 *)__first_795 + 16B], tmp1178
	leaq	1(%rax), %rsi	#, tmp908
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L241:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$32, %r13	#, __first
	jmp	.L240	#
.L364:
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	-1336(%rbp), %rdi	# MEM[(struct _Vector_base *)_1257]._M_impl.D.279431._M_start, _786
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	-1320(%rbp), %rsi	# MEM[(struct _Vector_base *)_1257]._M_impl.D.279431._M_end_of_storage, _787
	subq	%rdi, %rsi	# _786, _787
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%rdi, %rdi	# _786
	je	.L245	#,
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L245:
# ./script/menu/main_menu.h:195: struct LDCProcessState
	leaq	-1360(%rbp), %rdi	#, tmp913
	call	_ZNSt6vectorI23LDCSubsequentReferencesSaIS0_EED1Ev	#
# tmp_cmp.cpp:105: 	};
	movq	-1528(%rbp), %rbx	# %sfp, _204
.L247:
	subq	$32, %rbx	#, _204
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	(%rbx), %rdi	# MEM[(char * *)_204], _273
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	16(%rbx), %rax	#, tmp916
	cmpq	%rax, %rdi	# tmp916, _273
	je	.L246	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%rbx), %rax	# MEM <size_type> [(union ._anon_79 *)_204 + 16B], tmp1181
	leaq	1(%rax), %rsi	#, tmp917
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L246:
# tmp_cmp.cpp:185: }
	cmpq	-1520(%rbp), %rbx	# %sfp, _204
	jne	.L247	#,
	movq	%r12, %rdi	# tmp617,
.LEHB32:
	call	_Unwind_Resume@PLT	#
.L365:
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	(%r14), %rdi	# MEM[(struct basic_string * *)__first_783], _792
# /usr/include/c++/11/bits/stl_vector.h:336: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	16(%r14), %rsi	# MEM[(struct basic_string * *)__first_783 + 16B], _793
	subq	%rdi, %rsi	# _792, _793
# /usr/include/c++/11/bits/stl_vector.h:353: 	if (__p)
	testq	%rdi, %rdi	# _792
	je	.L243	#,
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L243:
# /usr/include/c++/11/bits/stl_construct.h:162: 	  for (; __first != __last; ++__first)
	addq	$24, %r14	#, __first
	jmp	.L239	#
.L227:
	leaq	-1024(%rbp), %rax	#, tmp926
	movq	%rax, -1520(%rbp)	# tmp926, %sfp
.L229:
# tmp_cmp.cpp:105: 	};
	subq	$32, %rbx	#, _227
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	(%rbx), %rdi	# MEM[(char * *)_227], _263
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	leaq	16(%rbx), %rax	#, tmp879
	cmpq	%rax, %rdi	# tmp879, _263
	je	.L228	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%rbx), %rax	# MEM <size_type> [(union ._anon_79 *)_227 + 16B], tmp1170
	leaq	1(%rax), %rsi	#, tmp880
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L228:
	cmpq	-1520(%rbp), %rbx	# %sfp, _227
	jne	.L229	#,
	movq	%r12, %rdi	# tmp883,
	call	_Unwind_Resume@PLT	#
.LEHE32:
.L230:
# /usr/include/c++/11/bits/basic_string.h:195:       { return _M_dataplus._M_p; }
	movq	-1232(%rbp), %rdi	# MEM[(struct basic_string *)_1253]._M_dataplus._M_p, _267
# /usr/include/c++/11/bits/basic_string.h:239: 	if (!_M_is_local())
	cmpq	%rbx, %rdi	# tmp936, _267
	je	.L231	#,
# /usr/include/c++/11/bits/basic_string.h:245:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	-1216(%rbp), %rax	# MEM[(struct basic_string *)_1253].D.25367._M_allocated_capacity, tmp1172
	leaq	1(%rax), %rsi	#, tmp887
# /usr/include/c++/11/ext/new_allocator.h:145: 	::operator delete(__p
	call	_ZdlPvm@PLT	#
.L231:
	movq	%r12, %rbx	# tmp889, tmp733
	jmp	.L168	#
	.cfi_endproc
.LFE13443:
	.section	.gcc_except_table
	.align 4
.LLSDAC13443:
	.byte	0xff
	.byte	0x9b
	.uleb128 .LLSDATTC13443-.LLSDATTDC13443
.LLSDATTDC13443:
	.byte	0x1
	.uleb128 .LLSDACSEC13443-.LLSDACSBC13443
.LLSDACSBC13443:
	.uleb128 .LEHB32-.LCOLDB25
	.uleb128 .LEHE32-.LEHB32
	.uleb128 0
	.uleb128 0
.LLSDACSEC13443:
	.byte	0x1
	.byte	0
	.align 4
	.long	0

.LLSDATTC13443:
	.section	.text.unlikely
	.text
	.size	_ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE, .-_ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE
	.section	.text.unlikely
	.size	_ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE.cold, .-_ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE.cold
.LCOLDE25:
	.text
.LHOTE25:
	.section	.text.startup
	.p2align 4
	.type	_GLOBAL__sub_I__ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE, @function
_GLOBAL__sub_I__ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE:
.LFB16550:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
1:	call	*mcount@GOTPCREL(%rip)
# tmp_cmp.cpp:185: }
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	jmp	_Z41__static_initialization_and_destruction_0ii.constprop.0	#
	.cfi_endproc
.LFE16550:
	.size	_GLOBAL__sub_I__ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE, .-_GLOBAL__sub_I__ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__ZN11LDCCompiler7compileEPKcRSt6vectorI10LDCClusterSaIS3_EE
	.weak	_ZGV5iKeys
	.section	.bss._ZGV5iKeys,"awG",@nobits,_ZGV5iKeys,comdat
	.align 8
	.type	_ZGV5iKeys, @gnu_unique_object
	.size	_ZGV5iKeys, 8
_ZGV5iKeys:
	.zero	8
	.local	_ZL9gCamera2D
	.comm	_ZL9gCamera2D,128,32
	.weak	_ZN4Init7iConfigE
	.section	.bss._ZN4Init7iConfigE,"awG",@nobits,_ZN4Init7iConfigE,comdat
	.align 32
	.type	_ZN4Init7iConfigE, @gnu_unique_object
	.size	_ZN4Init7iConfigE, 64
_ZN4Init7iConfigE:
	.zero	64
	.weak	iKeys
	.section	.bss.iKeys,"awG",@nobits,iKeys,comdat
	.align 16
	.type	iKeys, @gnu_unique_object
	.size	iKeys, 24
iKeys:
	.zero	24
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC16:
	.long	1144258560
	.align 4
.LC17:
	.long	1151336448
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC24:
	.quad	7088504621017299315
	.quad	8247620791347275877
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
