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
class TimedBoundedBuffer extends BoundedBuffer {
    int count;

    TimedBoundedBuffer (int maxSize) {
        super(maxSize);
        count = 0;
    }
    // This method must be protected against simultaneous accesses
    // from consumers. But **DO NOT CHANGE** the signature of this method.
    Object get() throws InterruptedException {
        Object value;
        synchronized(this) {
            // Suspend until an full slot is available
            while (count == 0) wait();
            value = super.get();
            // Release an empty slot
            while(count < maxSize) notify();
        }
        return value;
    }
    // This method must be protected against simultaneous accesses
    // from consumers.  But **DO NOT CHANGE** the signature of this method.
    Object get(long timeout) throws InterruptedException {
        Object value;
        long start = System.currentTimeMillis();
            // Suspend until an full slot is available. Resume when
            // timeout has expired.
            value = super.get();
        // Release an empty slot
        return value;
    }
    // This method must be protected against simultaneous accesses
    // from producers. But **DO NOT CHANGE** the signature of this method.
    void put(Object value) throws InterruptedException {
        synchronized(this) {
            // Suspend until an empty slot is available. Resume when
            while(count == maxSize) wait();
            super.put(value);
            // Release a full slot
            while(count > 0) notify();
        }
    }
    // This method must be protected against simultaneous accesses
    // from producers. But **DO NOT CHANGE** the signature of this method.
    void put(Object value, long timeout) throws InterruptedException {
        long start = System.currentTimeMillis();
            // Suspend until an empty slot is available. Resume when
            // timeout has expired.
            super.put(value);
            // Release a full slot
    }
}
