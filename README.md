# BreakoutCpp
Small project in C++/ SDL2/ TinyXML

*SDL2 and TinyXMl installed via vcpkg

XMLfiles are containing Level files. 
Format of level files: 
      
      <Level
        RowCount=""
        ColumnCount=""
        RowSpacing=""
        ColumnSpacing=""
        BackgroundTexture=""
        >
        
        <BrickTypes>
        <BrickType
        Id=""
        Texture=""
        HitPoints=""
        HitSound=""
        BreakSound=""
        BreakScore="" />
        </BrickTypes>
        
        <Bricks>
        example: S S S S S S S S S S S S S S       //"S" means brick type ID. " " means column spacing and _ means no brick there.
                 S S S S S _ _ _ _ S S S S S       //  Number of bricks possible in a row => Column count, in a column => rowCount
                 S S S S S _ _ _ _ S S S S S  	   // Level is loaded as drawn on here
        </Bricks>

        </Level>
        
There's no UI as this is just a demo project, so after win or lose it just restarts.

