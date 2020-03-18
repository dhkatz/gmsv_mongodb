require('mongodb')

client = client or mongodb('<URI>', '<APP NAME>')

-- Assuming you have a database named "garrysmod" and a collection called "players"
local players = client:Collection('garrysmod', 'players')

local bulk = players:Bulk()

for _, ply in ipairs(players.GetAll()) do
    bulk:Insert( _id = ply:SteamID64(), name = ply:Nick() )
end

bulk:Execute()
