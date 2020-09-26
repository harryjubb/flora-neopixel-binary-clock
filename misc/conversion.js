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

// Example time
const h1 = 0
const h2 = 5
const m1 = 5
const m2 = 5
const s1 = 0
const s2 = 6

// Concatenate the binary rep (with padding) of each clock digit together
const hms = parseInt(
    h1.toString(2).padStart(2, '0') +
    h2.toString(2).padStart(4, '0') +
    m1.toString(2).padStart(3, '0') +
    m2.toString(2).padStart(4, '0') +
    s1.toString(2).padStart(3, '0') +
    s2.toString(2).padStart(4, '0')
)
console.log(hms);

// Map of NeoPixel indices (list indices) to the bit they represent
pixelMap = [
  // Seconds small
  0b1000,
  0b0100,
  0b0010,
  0b0001,

  // Seconds large
  0b0010000,
  0b0100000,
  0b1000000,

  // Minutes small
  0b10000000000,
  0b01000000000,
  0b00100000000,
  0b00010000000,
  
  // Minutes large
  0b00100000000000,
  0b01000000000000,
  0b10000000000000,
  
  // Hours small
  0b100000000000000000,
  0b010000000000000000,
  0b001000000000000000,
  0b000100000000000000,
  
  // Hours large
  0b0100000000000000000,
  0b1000000000000000000,
]

for (let i = 0; i < pixelMap.length; i++) {
    console.log('')
    console.log(i)
    console.log(pixelMap[i])
    console.log(hms & pixelMap[i])
}