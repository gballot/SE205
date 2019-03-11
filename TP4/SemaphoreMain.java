/* Copyright (C) 1991-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
public class SemaphoreMain {
    public static void main(String[] args) {
       Semaphore sem;
       Thread agents [];
       int nAgents;
       int count;
       // Check arguments on command line
       if(args.length != 2) {
           System.out.println("PROGRAM N_AGENTS SEM_SIZE");
           System.exit(1);
       }
       nAgents = Integer.parseInt(args[0]);
       count = Integer.parseInt(args[1]);
       // Create semaphore
       sem = new Semaphore(count);
       // Allocate agent array
       agents = new Thread[nAgents];
       // Allocate agents
       // Start threads
       for (int i = 0 ; i < nAgents ; i++) {
           agents[i] = new Agent(i, sem);
           agents[i].start();
       }
       System.out.println("main terminated");
    }
}
