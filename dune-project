(lang dune 3.0)
(generate_opam_files true)

(name unisim_archisec)
(version "0.0.11")
(maintainers "BINSEC <binsec@saxifrage.saclay.cea.fr>")
(authors
  "Frédéric Recoules"
  "Yves Lhuillier"
)
(license BSD-3-Clause)
(homepage "https://binsec.github.io")
(source (github binsec/unisim_archisec))
(bug_reports "mailto:binsec@saxifrage.saclay.cea.fr")

(package
 (name unisim_archisec)
 (synopsis "UNISIM-VP DBA decoder")
 (description "
UNISIM ARCHISEC is a companion package of the binary analysis platform
BINSEC. It exposes disassembly metadata and DBA (Dynamic Bitvector Automata)
semantics of several instruction set architectures, including ARM, x86_64,
ppc64 and Sparcv8.")
(depends
  (conf-gcc :build)
  (conf-g++ :build))
 (conflicts
  (binsec (< 0.10.0))))
