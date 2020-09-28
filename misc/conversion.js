// Prototype code for time to binary clock conversion

/*
Always expecting a strip of 20 NeoPixels for a binary-coded decimal clock
Using a different strip length may need a different consideration for MAX_BRIGHTNESS

As diagrammed:

    8    8    8                                                          14      7      0
    4  4 4  4 4                                                          15  13  8   6  1
  2 2  2 2  2 2      Equivalent to NeoPixel strip array indices:      19 16  12  9   5  2
  1 1  1 1  1 1                                                       18 17  11 10   4  3

  H H  M M  S S                                                        H  H   M  M   S  S

*/

// Example time, as separate integers for each digit
const h1 = 0
const h2 = 5
const m1 = 5
const m2 = 5
const s1 = 0
const s2 = 6

// Convert these to bitstrings, padded with zeros
// to reach the length of each digit's binary representation on the clock
const h1b = h1.toString(2).padStart(2, '0')
const h2b = h2.toString(2).padStart(4, '0')
const m1b = m1.toString(2).padStart(3, '0')
const m2b = m2.toString(2).padStart(4, '0')
const s1b = s1.toString(2).padStart(3, '0')
const s2b = s2.toString(2).padStart(4, '0')

console.log(h1b)
console.log(h2b)
console.log(m1b)
console.log(m2b)
console.log(s1b)
console.log(s2b)
console.log()

// Concatenation of all of the binary representations of each digit, to make a single bitstring
const hmsb = (
  h1b +
  h2b +
  m1b +
  m2b +
  s1b +
  s2b
)

console.log(hmsb);
console.log()

// Numeric parse of the concatenated bitstring
const hms = parseInt(hmsb, 2)

console.log(hms)
console.log(hms.toString(2))
console.log()

// Map of NeoPixel indices (list indices) to the bit they represent
// in a concatenated bitstring representation of the clock positions
// Written out in full for clarity
pixelMap = [
  // Seconds small
  0b00000000000000001000, // == 0x8 == 8
  0b00000000000000000100, // == 0x4 == 4
  0b00000000000000000010, // == 0x2 == 2
  0b00000000000000000001, // == 0x1 == 1
  
  // Seconds large
  0b00000000000000010000, // == 0x10 == 16
  0b00000000000000100000, // == 0x20 == 32
  0b00000000000001000000, // == 0x40 == 64
  
  // Minutes small
  0b00000000010000000000, // == 0x400 == 1024
  0b00000000001000000000, // == 0x200 == 512
  0b00000000000100000000, // == 0x100 == 256
  0b00000000000010000000, // == 0x80 == 128
  
  // Minutes large
  0b00000000100000000000, // == 0x800 == 2048
  0b00000001000000000000, // == 0x1000 == 4096
  0b00000010000000000000, // == 0x2000 == 8192
  
  // Hours small
  0b00100000000000000000, // == 0x20000 == 131072
  0b00010000000000000000, // == 0x10000 == 65536
  0b00001000000000000000, // == 0x8000 == 32768
  0b00000100000000000000, // == 0x4000 == 16384
  
  // Hours large
  0b01000000000000000000, // == 0x40000 == 262144
  0b10000000000000000000, // == 0x80000 == 524288
]

// Bitwise AND to determine which pixel indices should be on or off
for (let i = 0; i < pixelMap.length; i++) {
    console.log('')
    console.log(i)
    console.log(pixelMap[i])
    console.log(hms & pixelMap[i])
}