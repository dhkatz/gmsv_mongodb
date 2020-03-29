require('mongodb')

client = client or mongodb.Client( '<URI>', '<APP NAME>' )

-- Assuming you have a database named "garrysmod" and a collection called "players"
local players = client:Collection( 'garrysmod', 'players' )

local bulk = players:Bulk()

-- Increment the money of every player by 100
for _, ply in ipairs( player.GetAll() ) do
    -- See https://docs.mongodb.com/manual/reference/operator/update/ for available update operators
    bulk:Update( { steamid = ply:SteamID64() }, { ['$inc'] = { money = 100 } } )
end

bulk:Execute()
