# gmsv_mongodb

A MongoDB module for Garry's Mod!

| Windows | Linux |
|---------|-------|
| [![Build status](https://ci.appveyor.com/api/projects/status/8ervbutov5w0144n/branch/development?svg=true)](https://ci.appveyor.com/project/dhkatz/gmsv-mongodb/branch/development)| [![Build Status](https://travis-ci.org/dhkatz/gmsv_mongodb.svg?branch=development)](https://travis-ci.org/dhkatz/gmsv_mongodb) |

## Quickstart

### Information

Why does Garry's Mod need a MongoDB module? Because MongoDB is a NoSQL database.
MySQL is designed for static schemas and performs best with complex queries.

Typically, the data stored by Garry's Mod servers is extremely dynamic 
and the queries performed are very simple. This makes MySQL a pain.

With MongoDB you don't have to worry about schemas at all! Just write
and query whatever data you want!

### Download

Download a pre-built binary for your system from the [Releases](https://github.com/dhkatz/gmsv_mongodb/releases) section.

Place the `.dll` in `garrysmod/lua/bin`.

## Usage

For more details see the [documentation](docs/README.md) or [examples](examples).

```lua
require('mongodb')

-- Note: Connecting to the MongoDB cluster can hang! Cache the result in a global!
client = client or mongodb.Client('<CONNECTION_URI>', '<APP_NAME>')

-- Retrieve databases
local database = client:Database('test')

-- Retrieve collections
local players = database:GetCollection('players')

-- Run queries (Find, Insert, Update, Remove)
local data = players:Find({ name = 'Bob', age = 25 })

print(data[1].name)

players:Update({ name = 'Bob' }, { ['$set'] = { age = 26 } })  

-- Execute bulk queries
local bulk = players:Bulk()

for i, ply in ipairs({ 'Bob', 'Jim', 'Ann' }) do
    bulk:Insert({ name = ply, age = 20 + i })
end 

local result = bulk:Execute()
```

## License

Code licensed under the [MIT License](LICENSE).
