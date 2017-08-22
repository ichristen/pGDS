-- Base credit to https://codereview.stackexchange.com/questions/83825/creating-2d-vectors

local Vector = {}
do
    local meta = {
        _metatable = "Private metatable",
        _DESCRIPTION = "Vectors in 2D"
    }

    meta.__index = meta

    function meta:__add( v )    -- Colon means that self is passed as 1st parameter.
        if tonumber(v) then     -- If v is a number
            return nil
        else                    -- If v is a vector
            return Vector(self.i + v.i, self.j + v.j)
        end
    end

    function meta:__mul( v )
        if tonumber(v) then     -- If v is a number
            return Vector(self.i * v, self.j * v)
        else                    -- If v is a vector
            return self.i * v.i + self.j * v.j
        end
    end

    function meta:__tostring()
        return ("<%g, %g>"):format(self.i, self.j)
    end

    function meta:magnitude()
        return math.sqrt( self * self )
    end

    setmetatable( Vector, {
        __call = function( V, i ,j ) return setmetatable( {i = i, j = j}, meta ) end
    } )
end

Vector.__index = Vector

return Vector
