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
import java.io.*;
import java.util.*;
class BarrierMain {
    static int nWorkers;
    static int nJobs;
    public static void main(String args[]){
        Parse(args);
        // Create synchronization structures
        Barrier AvailableJobs = new Barrier();
        Barrier ActivatedWorkers = new Barrier();
        Barrier CompletedWorkers = new Barrier();
        Barrier ActivatedThreads = new Barrier();
        Barrier CompletedThreads = new Barrier();
        // Initialize synchronization structures
        // Create and start Master
        new Master
            (AvailableJobs,
             ActivatedThreads,
             CompletedThreads,
             ActivatedWorkers,
             CompletedWorkers,
             nWorkers,
             nJobs).start();
        // Create and start workers
        for(int nWorker = 0; nWorker < nWorkers; nWorker++){
            new Worker
                (AvailableJobs,
                 ActivatedThreads,
                 CompletedThreads,
                 ActivatedWorkers,
                 CompletedWorkers,
                 nWorker).start();
        }
    }
    private static void Parse(String args[]){
        if (args.length != 2){
            System.out.println("PROGRAM <N_WORKERS> <N_JOBS>");
            System.exit(1);
        }
        nWorkers = Integer.parseInt(args[0]);
        nJobs = Integer.parseInt(args[1]);
        if (nWorkers <= 0) {
            System.out.println("There is no worker");
            System.exit(1);
        }
        if (nJobs <= 0) {
            System.out.println("There is no job");
            System.exit(1);
        }
        if (nJobs < nWorkers) {
            System.out.println("There are less jobs than workers");
            System.exit(1);
        }
    }
}
