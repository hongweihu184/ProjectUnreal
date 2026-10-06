# 一次性脚本:生成 Enhanced Input 资产(IA_Move/IA_Look/IA_Jump + IMC_Default)
# 用法(编辑器关闭时):
#   UnrealEditor-Cmd.exe <uproject> -run=pythonscript -script="Tools/setup_input.py"
import unreal

INPUT_DIR = "/Game/Input"
LOG = "[InputSetup]"

asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
eal = unreal.EditorAssetLibrary


def make_key(name):
    k = unreal.Key()
    k.import_text(name)
    return k


def make_ia(name, value_type):
    path = f"{INPUT_DIR}/{name}"
    if eal.does_asset_exist(path):
        unreal.log(f"{LOG} 已存在,跳过: {path}")
        return eal.load_asset(path)
    asset = asset_tools.create_asset(name, INPUT_DIR, unreal.InputAction, None)
    asset.set_editor_property("value_type", value_type)
    eal.save_asset(path)
    unreal.log(f"{LOG} 已创建: {path}")
    return asset


ia_move = make_ia("IA_Move", unreal.InputActionValueType.AXIS2D)
ia_look = make_ia("IA_Look", unreal.InputActionValueType.AXIS2D)
ia_jump = make_ia("IA_Jump", unreal.InputActionValueType.BOOLEAN)

# IMC 每次全量重建,保证映射干净
imc_path = f"{INPUT_DIR}/IMC_Default"
if eal.does_asset_exist(imc_path):
    eal.delete_asset(imc_path)
    unreal.log(f"{LOG} 旧 IMC 已删除,重建")
imc = asset_tools.create_asset("IMC_Default", INPUT_DIR, unreal.InputMappingContext, None)


def map_key(action, key_name, modifier_classes=()):
    mapping = imc.map_key(action, make_key(key_name))
    for mc in modifier_classes:
        mods = list(mapping.get_editor_property("modifiers"))
        mods.append(mc())
        mapping.set_editor_property("modifiers", mods)
    unreal.log(f"{LOG} 映射: {action.get_name()} <- {key_name} (+{len(modifier_classes)} 修改器)")


# bool 输入默认落在 X 轴;前后(W/S)需 Swizzle 转到 Y 轴,左(A)取反
map_key(ia_move, "D")
map_key(ia_move, "A", (unreal.InputModifierNegate,))
map_key(ia_move, "W", (unreal.InputModifierSwizzleAxis,))
map_key(ia_move, "S", (unreal.InputModifierSwizzleAxis, unreal.InputModifierNegate))
map_key(ia_look, "Mouse2D")
map_key(ia_jump, "SpaceBar")

eal.save_asset(imc_path)
unreal.log(f"{LOG} 完成")
