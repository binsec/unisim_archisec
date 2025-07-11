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

module Arm32 = struct
  external decode :
    thumb:bool -> bigendian:bool -> itstate:int -> addr:int32 -> int32 -> string
    = "arm32dba_decode"

  let decode ~thumb ?(bigendian = false) ?(itstate = 0) ~addr code =
    decode ~thumb ~bigendian ~itstate ~addr code
end

module Aarch64 = struct
  external decode : addr:int64 -> int32 -> string = "aarch64dba_decode"
end

module Amd64 = struct
  external decode : m64:bool -> addr:int64 -> string -> string
    = "amd64dba_decode"
end

module Ppc64 = struct
  external decode : addr:int64 -> int32 -> string = "ppc64dba_decode"
end

module Sparc = struct
  external decode : addr:int32 -> int32 -> int32 -> string = "sparcdba_decode"
end