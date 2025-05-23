#r "nuget: Newtonsoft.Json"

open System
open System.IO
open System.Runtime.InteropServices
open Newtonsoft.Json


module CInterop =
    [<DllImport("dist/clib/zfp_comp.so", EntryPoint="hello_world")>]
    extern void hello()

    [<DllImport("dist/clib/zfp_comp.so", EntryPoint="compress")>]
    extern IntPtr compress(double [], int , double , int&)

let json = File.ReadAllText("doubles.json")
let doubles = JsonConvert.DeserializeObject<double[]>(json)

CInterop.hello ()

let mutable outSize = 0
let ptr = CInterop.compress(doubles, 1562, 1e-1, &outSize)

let compressed = Array.zeroCreate<byte> outSize
Marshal.Copy(ptr, compressed, 0, outSize)

// Free buffer
Marshal.FreeHGlobal ptr

File.WriteAllBytes("doubles_test.zfp", compressed)

