# Collection

A MongoDB collection instance.

## Methods

### `string Client:Name()`

#### Description

Get the current name of the collection.

#### Example

```lua
local client = mongodb.Client('URI', 'Garry\'s Mod')

local col = client:Collection('garrysmod', 'players')

print(col:Name())

-- players
```

### `table Collection:Command(table command, table options)`

#### Description

Executes the passed command on the given collection and returns the result.

### `number Client:Count(table filter)`

#### Description

Return the number of documents matching a filter.

### `table Client:Find(table filter)`

#### Description

Find documents matching a filter.

### `table Client:FindOne(table filter)`

#### Description

Find the first document matching a filter.

### `bool Client:Insert(table document)`

#### Description

Insert a new document

### `bool Collection:Remove(table filter)`

#### Description

Remove documents matching a filter.

### `bool Collection:Update(table filter, table update)`

#### Description

Update documents with a set of update operators given a filter. 

### `Bulk Collection:Bulk()`

#### Description

Create a new [Bulk](Bulk.md) insert operation for the collection.
