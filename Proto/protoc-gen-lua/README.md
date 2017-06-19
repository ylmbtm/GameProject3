protoc-gen-lua
==============

Google's Protocol Buffers project, ported to Lua

"[Protocol Buffers](http://code.google.com/p/protobuf/)" is a binary serialization format and technology, released to the open source community by Google in 2008.

There are various implementations of Protocol Buffers and this is for Lua.

## Install

Install python runtime and the protobuf 2.3 for python.

checkout the code.

Compile the C code:

`$cd protobuf  && make`

Make a link to protoc-gen-lua  in your $PATH:

`$cd /usr/local/bin && sudo ln -s /path/to/protoc-gen-lua/plugin/protoc-gen-lua`

Then you can compile the .proto like this:

`protoc --lua_out=./ foo.proto`


## Quick Example
You write a .proto file like this:

person.proto :
```
  message Person {
    required int32 id = 1;
    required string name = 2;
    optional string email = 3;
  }
```

Then you compile it.

Then,  make sure that protobuf/ in package.cpath and package.path,  you use that code like this:

```
require "person_pb"

-- Serialize Example
local msg = person_pb.Person()
msg.id = 100
msg.name = "foo"
msg.email = "bar"
local pb_data = msg:SerializeToString()

-- Parse Example
local msg = person_pb.Person()
msg:ParseFromString(pb_data)
print(msg.id, msg.name, msg.email)
```

The API of this library is similar the protobuf library for python.
For a more complete example,  read the [python documentation](http://code.google.com/apis/protocolbuffers/docs/pythontutorial.html).
