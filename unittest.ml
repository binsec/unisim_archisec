(******************************************************************************)
(*  Copyright (c) 2007-2021,                                                  *)
(*  Commissariat a l'Énergie Atomique et aux Énergies Alternatives (CEA)      *)
(*  All rights reserved.                                                      *)
(*                                                                            *)
(*  Redistribution and use in source and binary forms, with or without        *)
(*  modification, are permitted provided that the following conditions        *)
(*  are met:                                                                  *)
(*                                                                            *)
(*   - Redistributions of source code must retain the above copyright         *)
(*     notice, this list of conditions and the following disclaimer.          *)
(*                                                                            *)
(*   - Redistributions in binary form must reproduce the above copyright      *)
(*     notice, this list of conditions and the following disclaimer in the    *)
(*     documentation and/or other materials provided with the distribution.   *)
(*                                                                            *)
(*   - Neither the name of CEA nor the names of its contributors may be used  *)
(*     to endorse or promote products derived from this software without      *)
(*     specific prior written permission.                                     *)
(*                                                                            *)
(*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *)
(*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *)
(*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     *)
(*  A PARTICULAR PURPOSE ARE DISCLAIMED.                                      *)
(*  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY   *)
(*  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL        *)
(*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS   *)
(*  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)     *)
(*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       *)
(*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING     *)
(*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE        *)
(*  POSSIBILITY OF SUCH DAMAGE.                                               *)
(******************************************************************************)

let () =
  print_string "Amd64dba.decode ~m64:false ~addr:0x4000L \"55\"";
  print_newline ();
  print_string @@ Amd64dba.decode ~m64:false ~addr:0x4000L "55";
  print_newline ();
  print_string "Amd64dba.decode ~m64:true ~addr:0x4000L \"55\"";
  print_newline ();
  print_string @@ Amd64dba.decode ~m64:true ~addr:0x4000L "55";
  print_newline ();
  print_string "Amd64dba.decode ~m64:true ~addr:0x4000L \"30c0\"";
  print_newline ();
  print_string @@ Amd64dba.decode ~m64:true ~addr:0x4000L "30c0";
  print_newline ();
  print_string "Arm32dba.decode ~thumb:false ~addr:0x4000l 0xe2543210l";
  print_newline ();
  print_string @@ Arm32dba.decode ~thumb:false ~addr:0x4000l 0xe2543210l;
  print_newline ();
  print_string "Arm32dba.decode ~thumb:true ~addr:0x4000l 0x000af04fl";
  print_newline ();
  print_string @@ Arm32dba.decode ~thumb:true ~addr:0x4000l 0x000af04fl;
  print_newline ();
  print_string "Aarch64dba.decode ~addr:0x4000L 0x18000020l";
  print_newline ();
  print_string @@ Aarch64dba.decode ~addr:0x4000L 0x18000020l;
  print_newline ();
  print_string "Ppc64dba.decode ~addr:0x4000L 0x7c011214l";
  print_newline ();
  print_string @@ Ppc64dba.decode ~addr:0x4000L 0x7c011214l;
  print_newline ();
  print_string "Sparcdba.decode ~addr:0x4000L 0x82186008l 0l";
  print_newline ();
  print_string @@ Sparcdba.decode ~addr:0x4000l 0x82186008l 0l;
  print_newline ();
  print_string "Sparcdba.decode ~addr:0x4000L 0xae15e3a0l 0l";
  print_newline ();
  print_string @@ Sparcdba.decode ~addr:0x4000l 0xae15e3a0l 0l;
  print_newline ();
  print_string "Sparcdba.decode ~addr:0x4000L 0xc25dc001l 0l";
  print_newline ();
  print_string @@ Sparcdba.decode ~addr:0x4000l 0xc205c001l 0l
