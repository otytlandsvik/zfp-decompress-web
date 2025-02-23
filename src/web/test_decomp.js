/**
 * Decompress the compressed data given by `compressedData`. Assumes target is 1d double array.
 * @param {compressedData} Uint8Array containing the compressed data
 * @param {originalLength} the original size of the uncompressed double array
 * @param {tolerance} Inaccuracy tolerance used in compression
 * @returns {Float64Array} Decompressed data
 */
async function decompress(compressedData, originalLength, tolerance) {
  // Wait for wasm module to load
  const Zfp = await Module();

  // malloc and memcpy compressed array into wasm heap
  let compressed_p = Zfp._malloc(compressedData.length);
  Zfp.HEAP8.set(compressedData, compressed_p);

  // Run decompression
  let new_data_p = Zfp._decompress(
    compressed_p,
    compressedData.length,
    originalLength,
    tolerance,
  );

  // Retrieve uncompressed data from wasm heap
  return Zfp.HEAPF64.subarray(new_data_p / 8, new_data_p / 8 + originalLength);
}
