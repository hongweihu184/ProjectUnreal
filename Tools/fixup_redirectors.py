import unreal

LOG = "[Fixup]"
eal = unreal.EditorAssetLibrary
tools = unreal.AssetToolsHelpers.get_asset_tools()

redirs = []
for p in ["/Game/Gameplay/BP_BaseCharacter",
          "/Game/Gameplay/GameplayAbility/GA_Attack_Light_1"]:
    if eal.does_asset_exist(p):
        obj = eal.load_asset(p)
        cls = obj.get_class().get_name() if obj else "?"
        unreal.log(f"{LOG} {p} 类型: {cls}")
        if cls == "ObjectRedirector":
            redirs.append(obj)

if redirs:
    try:
        tools.fixup_redirectors(redirs)
        unreal.log(f"{LOG} fixup 完成,处理 {len(redirs)} 个重定向器")
    except Exception as e:
        unreal.log(f"{LOG} fixup 失败: {e}")
else:
    unreal.log(f"{LOG} 无重定向器")

# 验证 BP 引用是否已指向新路径
bp = eal.load_asset("/Game/Gameplay/Characters/BP_BaseCharacter")
unreal.log(f"{LOG} BP 存在: {bp is not None}")
unreal.log(f"{LOG} 旧 BP 重定向器还在: {eal.does_asset_exist('/Game/Gameplay/BP_BaseCharacter')}")
