# Client

A MongoDB client instance.

## Methods

### `string Client:Uri()`

#### Description

Get the current URI of the connected client.

#### Example

```lua
local client = mongodb('URI', 'Garry\'s Mod')

print(client:Uri())

-- mongodb+srv://username:password@cluster.mongodb.net/test?retryWrites=true&w=majority
```

### `table Client:Command(string database, table command)`

#### Description

Executes the passed command on the given database and returns the result.

### `table Client:DatabaseList()`

#### Description

Return a list of databases the client has access to.

### `Database Database(string name)`

Get a specific [Database](Database.md) instance.
