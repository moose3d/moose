# ---------------------------------------------------------------------------------------
set g_Models {}  

# Maps texture file into resourcename set first
set g_lstTexNameCache {}
# Maps texture names into existing textures if same
set g_lstTexNameMap {}
# Texture resources that have not yet been loaded.
set g_lstNotCreatedTextures {}
# Maps all model names that use same model file.
# eg. { file1 { name1 name2 name3 ... } file2 { name4 name5 name6} ... } 
set g_lstModelNameCache {}

set g_lstKnownModelParams { .models .group }
set g_lstKnownTextureParams { .diffuse .bump .specular .occlusion }
 
array set g_TexParamToIndex { 
    .diffuse 0 
    .bump 1 
    .specular 2 
    .occlusion 3 
}

set g_lstShaders {}

# Maps resource name to proper class and script 
set g_lstSkeletons {}

# ---------------------------------------------------------------------------------------
proc AddCollider { name lstData } {
    
    if { [llength $lstData] > 2 } {
        CCompoundCollider c;
        foreach { type data } $lstData {
            if { [string equal $type sphere] } {
                
                array set p $data
                CSphereCollider sc;
                [sc GetBoundingSphere] SetPosition [lindex $p(.position) 0] [lindex $p(.position) 1] [lindex $p(.position) 2]
                [sc GetBoundingSphere] SetRadius $p(.radius)
                c AddCollider sc
                sc -disown
            } elseif { [string equal $type box] } {
                array set p $data
                COBBCollider bc
                [bc GetBoundingBox] SetPosition [lindex $p(.position) 0] [lindex $p(.position) 1] [lindex $p(.position) 2]
                [bc GetBoundingBox] SetWidth [lindex $p(.dimensions) 0]
                [bc GetBoundingBox] SetHeight [lindex $p(.dimensions) 1]
                [bc GetBoundingBox] SetLength [lindex $p(.dimensions) 2]
                c AddCollider bc
                bc -disown
            }
        }
        if { [[g_ColliderMgr] Create c $name] != 0 } {
            error "Could not create compund collider resource $name"
        } else {
            puts "created compound collider $name"
            c -disown
        }
    } else {
        
        if { [string equal [lindex $lstData 0] sphere] } {
                
            array set p [lindex $lstData 1]
            CSphereCollider sc
            [sc GetBoundingSphere] SetPosition [lindex $p(.position) 0] [lindex $p(.position) 1] [lindex $p(.position) 2]
            [sc GetBoundingSphere] SetRadius $p(.radius)
            if { [[g_ColliderMgr] Create sc $name] != 0 } {
                error "Could not create sphere collider resource $name"
            } else {
                puts "created sphere collider $name"
                sc -disown
            }
        } elseif { [string equal [lindex $lstData 0] box] } {
            array set p [lindex $lstData 1]
            COBBCollider bc
            [bc GetBoundingBox] SetPosition $p(.position)
            [bc GetBoundingBox] SetWidth [lindex $p(.dimensions) 0]
            [bc GetBoundingBox] SetHeight [lindex $p(.dimensions) 1]
            [bc GetBoundingBox] SetLength [lindex $p(.dimensions) 2]
            if { [[g_ColliderMgr] Create bc $name] != 0 } {
                error "Could not create box collider resource $name"
            } else {
                puts "created box collider $name"
                bc -disown
            }
        }
        
    }
}

proc AppendToModelNameCache { file paramList } {
    global g_lstModelNameCache

    array set aModelCache $g_lstModelNameCache
    
    if { [ llength  [ array get aModelCache $file ] ] == 0 } { 
        # insert new element 
        lappend g_lstModelNameCache $file $paramList
    } else {
        puts "Found existing file reference..." 
        set lstModels $aModelCache($file) 
        # Add new name to list 
        array unset aModelCache $file
        lappend lstModels $paramList 
        
        # reconstruct list
        set g_lstModelNameCache {}
           
        foreach { f n } [array get aModelCache ] {
            lappend g_lstModelNameCache $f $n        
        }
        lappend g_lstModelNameCache $file $lstModels
    }
}
proc IsModelFileLoaded { file } {
    global g_lstModelNameCache
    array set aModelCache $g_lstModelNameCache
    # File has at least one reference
    if { [llength [array get aModelCache $file]] > 0 } {
        # At least one model using this file is created
        foreach { modelname } $aModelCache($file) {
            if { [[GetModelMgr] HasResource $modelname] } {
                return 1
            }
        }
    } 
    return 0
}

proc NewShader { resourceName paramList } {
    global g_lstShaders
    lappend g_lstShaders $resourceName $paramList 
}

proc LoadShaders {} {
    global g_lstShaders
    array set s $g_lstShaders
    
    foreach { shader } [array names s] {

        if { [[g_ShaderMgr] HasResource $shader] } {
            continue
        }

        array set tmp_params $s($shader)     
        set tmp_vert ""
        set tmp_frag ""

        if { [llength [ array get tmp_params .vertex ]] > 0  } {
              set tmp_vert $tmp_params(.vertex)
        }
        if { [llength [ array get tmp_params .fragment ]] > 0  } {
              set tmp_frag $tmp_params(.fragment)
        }
        puts -nonewline "loading shader $shader..."
        LoadShader $tmp_vert $tmp_frag $shader
        puts "done!"
    }    
    
}

proc SetModelTextureCached { model index texturefile texname } {
    
    global g_lstTexNameCache g_lstTexNameMap g_lstNotCreatedTextures
    array set tmp $g_lstTexNameCache
    puts "$model $index $texturefile $texname"
    if { [ llength [ array get tmp $texturefile ] ] == 0 } { 
        # First time that this texture is loaded.
        LoadTexture2D $texturefile $texname
        lappend g_lstTexNameCache $texturefile $texname
        lappend g_lstNotCreatedTextures $texname
        puts "Loading new texture $texturefile -> $texname..."
        
    } else {
        # Use previous name from this resource
        lappend g_lstTexNameMap $texname  $tmp($texturefile)
        puts "Using cached texture name $tmp($texturefile)..."
    }

} 

proc AddModel { model lod id renderstate } {
    global g_lstTexNameMap
     
    array set aRenderState $renderstate
    array set aTexNames $g_lstTexNameMap

    set diffuse ""
    if { [ llength [ array get aRenderState .diffuse ] ] > 0 } { 
        set diffuse $aRenderState(.diffuse)
    }
    
    #puts "diffuse now $diffuse"
    if { [ llength [ array get aTexNames $diffuse ] ] > 0 } {
        set diffuseName $aTexNames($diffuse)
        array unset aRenderState .diffuse    
        set renderstate {}
        foreach { k v } [ array get aRenderState ] {
            lappend renderstate $k $v
        }
        lappend renderstate .diffuse $aTexNames($diffuseName)
    }
    #puts $renderstate 
    #puts "state length : [ llength $renderstate ] "
    UseModel $model $lod $id $renderstate
}

proc AddSharedModel { pGameObject modelName lod id } {
    set pRenderable [ $pGameObject AddRenderableModel $modelName $lod [$pGameObject GetWorldTransform]]
    $pRenderable SetId $id
    set pRs [$pRenderable GetRenderState]
    $pRs SetShader "moose_default_shader"

    $pRs AddShaderAttrib "a_vertex"   [[g_Models $modelName] GetVertexHandle]
    $pRs AddShaderAttrib "a_texcoord" [[g_Models $modelName] GetTextureCoordinateHandle 0]
    $pRs AddShaderUniform "diffuse"   0

    if { [$pRs Prepare] == 0 } {
        puts "Prepare error"
    } else {
        puts "prepare ok!"
    }
    #if { [[g_Models $modelName] GetTextureCoordinateHandle 1] IsNull} {
    #    Add under some predefined name into shader
    #}
    # $pRs AddShaderUniform "material" [$pRs GetMaterial 0]
    return $pRenderable
}

proc NewModel { name params } {
    global g_Models
 
    set new_params {}
    foreach { pname pvalue } $params {
        lappend new_params $name$pname $pvalue
    }
    # Check is model already declared
    array set m $g_Models 
    if { [llength [array get m $name]] == 0 } {
        lappend g_Models $name $new_params        
    } else {
        puts "*** Model $name already registered"
        return 
    }
    array unset m
    # Check whether our new model file has already been cached
    array set m $g_Models 
    array set tmp_params $m($name)
    if { [llength [array get m $name]] > 0 } {
        # Add cache, since it was not found (.model is filename suffix!)
        AppendToModelNameCache $tmp_params(${name}.model) $name
    }
}

proc NewTexture { texname propertyList } {

    global g_lstTexNameCache g_lstTexNameMap g_lstNotCreatedTextures
    array set tmp $g_lstTexNameCache
    array set props $propertyList

    if { [ llength [ array get tmp $props(.file) ] ] == 0 } { 
        # First time that this texture is loaded.
        switch $props(.type) {
            2d {
                LoadTexture2D $props(.file) $texname
            }
            cube {
                LoadTextureCube [list $props(.left) $props(.right) $props(.top) $props(.bottom) $props(.back) $props(.front)] $texname
            }
        }
        lappend g_lstTexNameCache $props(.file) $texname
        lappend g_lstTexNameMap $texname $texname
        lappend g_lstNotCreatedTextures $texname
        puts "Loading new texture $props(.file) -> $texname..."
        
    } else {
        # Use previous name from this resource
        lappend g_lstTexNameMap $texname $tmp($props(.file))
        puts "Using cached texture $texname = $tmp($props(.file))..."
    }     
}

# Call this only from renderer thread!
proc CreateTextures {} {
    global g_lstNotCreatedTextures
    #puts "calling createtextures..... $g_lstNotCreatedTextures"
    foreach { texname } $g_lstNotCreatedTextures {

        if { [g_Textures $texname] == "NULL" } {
            # Make texture object
            if { [g_TextureData $texname] != "NULL" } {
                set texture [ [g_TextureData $texname] CreateTexture [[g_Application] GetRenderer] ]
                puts "-> Creating texture $texname..."
                [g_Textures] Create $texture $texname
                # Release data, it is already converted into proper texture
                [g_TextureData] Destroy $texname
            }
        }
    }
    set g_lstNotCreatedTextures {}
}

proc LoadModels {} {
    global g_Models g_lstKnownModelParams  g_lstModelNameCache  g_ResourceAlias

    # Create array from list 
    array set m $g_Models 

    # Find out what models haven't been loaded yet
    set lstNotLoadedModels {}
    foreach { modelname } [array names m] {
        if { [[GetModelMgr] HasResource ${modelname}] == 1} {
            continue
        } else {
            lappend lstNotLoadedModels $modelname
        }
    }
    if { [llength $lstNotLoadedModels] == 0 } {
        puts "Nothing to load, backing off."
        return
    }
    # Remove duplicates
    set lstNotLoadedModels [lsort -unique $lstNotLoadedModels]
    
    set firstModel ""    
    array set c $g_lstModelNameCache

    # Check has model data been loaded earlier
    foreach { modelname } $lstNotLoadedModels {
        puts stderr "Processing $modelname"
        array set model_params $m($modelname)
        set file $model_params(${modelname}.model)
        puts "$modelname file: $file"
        
        puts "Processing file : $file with [llength $c($file)] references..."
        
        LoadModelFile $file
        puts -nonewline "-> $modelname ..."    
        
        set modelGroups ""  
        set interleavedData 0

        if { [llength [ array get model_params ${modelname}.group ] ] > 0 } {
            set modelGroups $model_params(${modelname}.group)
        }
        
        if { [llength [ array get model_params ${modelname}.interleaved ] ] > 0 } {
            set interleavedData $model_params(${modelname}.interleaved)
        }
        
        # Register model textures
        if { [llength [ array get model_params ${modelname}.diffuse ] ] > 0 } {
            NewTexture ${modelname}.diffuse [list .file $model_params(${modelname}.diffuse) .type 2d]
        }

        if { [llength $modelGroups] > 0 } {
            foreach { group } $modelGroups {
                # Create model from currently loaded data 
                CreateModel [ list ${modelname} [ list 1 2 4 8 ] $group $interleavedData ]
                # Compute bounding sphere for this model.
                ComputeBoundingSphere ${modelname} ${modelname}.boundingSphere
            }
        } else {
            CreateModel [ list ${modelname} [ list 1 2 4 8 ] "" $interleavedData ]
            # Compute bounding sphere for this model.
            ComputeBoundingSphere ${modelname} ${modelname}.boundingSphere
        }
        puts "-> Model $modelname done."            
    }
}

proc LoadTextures {} {
    global g_lstKnownTextureParams g_TexParamToIndex g_lstTexNameCache
    # Loads only texture data, actual texture obejct is created later.
    # \TODO Does not work yet!!!
    foreach p $g_lstKnownTextureParams {
        foreach { texparmname file } [ array get model_params ${modelname}${p} ] {
            SetModelTextureCached ${modelname} $g_TexParamToIndex($p) $file $texparmname
        }   
    }
}

proc Scale { vector value } {
    set scaledVec {}
    foreach { i } $vector {
        lappend scaledVec [ expr $i * $value ] 
    }
    return $scaledVec
}

proc Sum { vA vB } {
    set result {}
    set len [llength $vA]
    for { set i 0 } { $i < $len } { incr i } {
      set a [ lindex $vA $i ]
      set b [ lindex $vB $i ]
      lappend result [expr $a + $b ]
        
    }
    return $result
}

proc Sub { vA vB } {
    set result {}
    set len [llength $vA]
    for { set i 0 } { $i < $len } { incr i } {
      set a [ lindex $vA $i ]
      set b [ lindex $vB $i ]
      lappend result [expr $a - $b ]
        
    }
    return $result
}


proc AddSkeleton { name params } {
    global g_lstSkeletons 
    array set skels $g_lstSkeletons
    if { [llength [array get skels $name]] == 0 } {
        lappend g_lstSkeletons $name $params
    } else {
        puts "Object skeleton $name already exists, I will leave it be."
    }
    puts "skeletons: [llength $g_lstSkeletons]"
    
}

proc Instantiate { skeleton name pos rotation } {
    global g_lstSkeletons g_lstTexNameMap g_Models
    set scene [GetScene]
    array set skels $g_lstSkeletons

    if { [llength [array get skels $skeleton]] == 0 } {
        error "Cannot instatiate skeleton $skeleton. Not loaded?"
    } else {
        puts "*** Found skeleton $skeleton"
    }

    # Determine skeleton class
    array set skel $skels($skeleton)
    set class $skel(.class)
    array set textures $g_lstTexNameMap
    
    set obj [$class]
    # Initialize renderables if explicitly asked    
    if { [llength [array get skel .numLods]] > 0 } {
        $obj InitializeRenderables $skel(.numLods)
    } else {
        puts "-> $skeleton has no lod count, using 1."
        $obj InitializeRenderables 1
    }
    # Create object and add to scene
    $obj SetName $name
    if { [llength [array get skel .script]] > 0 } {
        $obj SetScript $skel(.script)
    }
    $scene AddGameObject $obj

    #  Object is now managed by Moose, so let it go from script.
    $obj -disown
    $obj SetChanged 1
     
    # --------------------
    # Add renderable to our object from model file, 
    # setup renderstate and shaders
    # --------------------
    if { [llength [array get skel .models]] > 0 } {
        puts "Skeleton has [llength $skel(.models)] models"

        set s [CSphere]
        $s SetRadius 0.0
        $s SetPosition 0.0 0.0 0.0


        foreach {m} $skel(.models) {
            puts "Processing model $m"
            # Merge spheres from all models
            set s [MergeSpheres $s [g_Spheres $m.boundingSphere]]
            
            set r [$obj AddRenderableModel $m 0 [$obj GetWorldTransform]]
            puts "done here.. $r"
            set rs [$r GetRenderState]
            
            array set tmpModels $g_Models
            #puts "params: [array get tmpModels]"
            array set tmpParams $tmpModels($m)

            #puts "HEYYYYYYY would load $tmpParams($m.shader)"
            # Load specific shader if parameter is defined
            # Otherwise, stick to default.
            if { [llength [array get tmpParams $m.shader]] > 0 } {
                $rs SetShader $tmpParams($m.shader)
            } else {
                $rs SetShader "moose_default_shader"
            }
            [$rs GetMaterial] SetSpecular [new_CVector4f 1.0 1.0 1.0 1.0]
            [$rs GetMaterial] SetDiffuse [new_CVector4f 0.7 0.7 0.7 1.0]
            [$rs GetMaterial] SetAmbient [new_CVector4f 0.27 0.27 0.27 1.0]
            [$rs GetMaterial] SetShininess 128.0

            #puts "I should use texture: ${m}.diffuse $textures(${m}.diffuse)"
            if { [llength [array get textures ${m}.diffuse]] > 0 } {
                $rs SetTexture 0 $textures(${m}.diffuse)
            }


            $rs AddShaderAttrib "a_vertex" [[g_Models $m] GetVertexHandle]
            if { [[[g_Models $m] GetNormalHandle] IsNull] == 0 } {
                $rs AddShaderAttrib "a_normal" [[g_Models $m] GetNormalHandle]
            }
            $rs AddShaderAttrib "a_texcoord" [[g_Models $m] GetTextureCoordinateHandle]
            $rs AddShaderUniform "m_globalAmbient" [new_CVector4f 0.2 0.2 0.2 1.0]
            $rs AddShaderUniform diffusetex 0
            
            $rs SetDepthTest 1
            $rs SetDepthWrite 1
            $rs Prepare   
        }
        $obj SetBoundingSphere $s
    }
    # Translate object to proper location
    [ $obj GetLocalTransform ] SetTranslation [ lindex $pos 0 ] [ lindex $pos 1 ] [ lindex $pos 2 ]
    [ $obj GetLocalTransform ] SetRotation [Deg2Rad [lindex $rotation 0]] [Deg2Rad [lindex $rotation 1]] [Deg2Rad [lindex $rotation 2]]
    # Execute post-instantiate script
	if { [llength [array get skel .postexec]] != 0 } {
        $skel(.postexec) $obj
    }
    # Return object
    return $obj
}    

proc g_AssetBundle { {name ""} } {
    if { $name == "" } {
        return [GetAssetBundle]
    } else {
        return [[GetAssetBundle] GetAssetPath $name]
    }
}

proc g_ObjectMgr { { name "" } } {
    if { $name == "" } {
        return [GetObjectMgr]
    } else {
        return [[GetObjectMgr] GetResource $name]
    }
}

proc g_TextureMgr { { name "" } } {
    if { $name == "" } {
        return [GetTextureMgr]
    } else {
        return [[GetTextureMgr] GetResource $name]
    }
}

proc g_TextureDataMgr { { name "" } } {
    if { $name == "" } {
        return [GetTextureDataMgr]
    } else {
        return [[GetTextureDataMgr] GetResource $name]
    }
}

proc g_VertexMgr { { name "" } } {
    if { $name == "" } {
        return [GetVertexMgr]
    } else {
        return [[GetVertexMgr] GetResource $name]
    }
}

proc g_IndexMgr { { name "" } } {
    if { $name == "" } {
        return [GetIndexMgr]
    } else {
        return [[GetIndexMgr] GetResource $name]
    }
}

proc g_ShaderMgr { { name "" } } {
    if { $name == "" } {
        return [GetShaderMgr]
    } else {
        return [[GetShaderMgr] GetResource $name]
    }
}

proc g_ModelMgr { { name "" } } {
    if { $name == "" } {
        return [GetModelMgr]
    } else {
        return [[GetModelMgr] GetResource $name]
    }
}    

proc g_ModelHelper {} {
    return [GetModelHelper]
}

proc g_CameraMgr { { name "" } } {
    if { $name == "" } {
        return [GetCameraMgr]
    } else {
        return [[GetCameraMgr] GetResource $name]
    }
}

proc g_ColliderMgr { { name "" } } {
    if { $name == "" } {
        return [GetColliderMgr]
    } else {
        return [[GetColliderMgr] GetResource $name]
    }
}

proc g_UniqueName { { prefix "" } } {
    if { $prefix == "" } {
        return [[GetNameCreator] GetUniqueName]
    } else {
        return [[GetNameCreator] GetUniqueName $prefix]
    }
}

proc g_SphereMgr { { name "" } } {
    if { $name == "" } {
        return [GetSphereMgr]
    } else {
        return [[GetSphereMgr] GetResource $name]
    }
}

proc g_AudioSystem {} {
    return [GetAudioSystem]
}

proc g_SampleMgr { { name "" } } {
    if { $name == "" } {
        return [GetSoundSampleMgr]
    } else {
        return [[GetSoundSampleMgr] GetResource $name]
    }
}

proc g_StreamMgr { { name "" } } {
    if { $name == "" } {
        return [GetSoundStreamMgr]
    } else {
        return [[GetSoundStreamMgr] GetResource $name]
    }
}

# More fancy naming
proc g_Objects { { name "" } } {
    return [g_ObjectMgr $name]
}

proc g_Textures { { name ""} } {
    return [g_TextureMgr $name ]
}

proc g_TextureData { { name "" } } {
    return [g_TextureDataMgr $name]
}

proc g_Vertices { { name "" } } {
    return [g_VertexMgr $name]
}

proc g_Indices { {name "" } } {
    return [g_IndexMgr $name]
}

proc g_Shaders { {name "" } } {
    return [g_ShaderMgr $name]
}

proc g_Models { {name "" } } {
    return [g_ModelMgr $name]
}    

proc g_Cameras { {name ""} } {
    return [g_CameraMgr $name]
}

proc g_Colliders { {name ""} } {
    return [g_ColliderMgr $name]
}

proc g_Samples { {name ""} } {
    return [g_SampleMgr $name]
}

proc g_Streams { {name ""} } {
    return [g_StreamMgr $name]
}

proc g_Application {} {
    return [GetApplication]
}

proc GetScene { { name "" } } {
    if { $name == "" } {
        return [[GetApplication] GetCurrentScene]
    } else {
        return [[GetApplication] GetScene $name]
    }
}

proc g_Spheres { {name ""} } {
    return [g_SphereMgr $name]
}