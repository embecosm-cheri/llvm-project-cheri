// RUN: %clang_cc1 -triple cheri-unknown-freebsd -target-abi purecap  -fsyntax-only -verify %s

class a {
public:
  typedef *a::*b; // expected-error{{C++ requires a type specifier for all declarations}}
  class foo {
    bool operator==(const foo& other){
      return other.c == c;
    }
    b c;
  };
};

