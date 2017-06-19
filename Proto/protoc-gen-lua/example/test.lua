package.path = package.path .. ';../protobuf/?.lua'
package.cpath = package.cpath .. ';../protobuf/?.so'

require 'person_pb'

local person= person_pb.Person()
person.id = 1000
person.name = "Alice"
person.email = "Alice@example.com"

local home = person.Extensions[person_pb.Phone.phones]:add()
home.num = "2147483647"
home.type = person_pb.Phone.HOME

local data = person:SerializeToString()

local msg = person_pb.Person()

msg:ParseFromString(data)
print(msg)
