import unreal

LOG = "[MapMove3]"
tools = unreal.AssetToolsHelpers.get_asset_tools()
eal = unreal.EditorAssetLibrary

world = eal.load_asset("/Game/NewMap")
if not world:
    raise Exception(f"{LOG} 加载 /Game/NewMap 失败")

data = unreal.AssetRenameData(world, "/Game/Maps", "L_TestGAS")
ok = tools.rename_assets([data])
unreal.log(f"{LOG} rename_assets 结果: {ok}")
unreal.log(f"{LOG} /Game/Maps/L_TestGAS 存在: {eal.does_asset_exist('/Game/Maps/L_TestGAS')}")
unreal.log(f"{LOG} /Game/NewMap 存在: {eal.does_asset_exist('/Game/NewMap')}")
