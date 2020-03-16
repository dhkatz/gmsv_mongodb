# gmsv_mongodb

A MongoDB module for Garry's Mod!

| Windows | Linux |
|---------|-------|
| [![Build status](https://ci.appveyor.com/api/projects/status/8ervbutov5w0144n/branch/development?svg=true)](https://ci.appveyor.com/project/dhkatz/gmsv-mongodb/branch/development)| [![Build Status](https://travis-ci.org/dhkatz/gmsv_mongodb.svg?branch=development)](https://travis-ci.org/dhkatz/gmsv_mongodb) |

## Quickstart

Download a pre-built binary for your system from the [Releases](https://github.com/dhkatz/gmsv_mongodb/releases) section.

Place the `.dll` in `garrysmod/lua/bin`.

## Usage

For more details see the [documentation](docs/README.md).

```lua
require('mongodb')

-- Note: Connecting to the MongoDB cluster can hang! Cache the result in a global!
client = client or mongodb('<CONNECTION_URI>', '<APP_NAME>')

local database = client:Database('test')

local players = database:GetCollection('players')

local data = players:Find({ name = 'Bob' })

print(data[0].name)

```

## License

Code licensed under the [MIT License](LICENSE).
