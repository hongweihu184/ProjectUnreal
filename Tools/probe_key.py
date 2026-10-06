import unreal

LOG = "[Probe2]"

k = unreal.Key()
try:
    k.import_text("W")
    unreal.log(f"{LOG} 实例 import_text 后 to_tuple: {k.to_tuple()}")
except Exception as e:
    unreal.log(f"{LOG} 实例 import_text 失败: {e}")

try:
    k2 = unreal.Key.import_text("A")
    unreal.log(f"{LOG} 静态 import_text OK: {k2}")
except Exception as e:
    unreal.log(f"{LOG} 静态 import_text 失败: {e}")

try:
    unreal.log(f"{LOG} 空 Key 的 editor properties 尝试 key_name: {k.get_editor_property('key_name')}")
except Exception as e:
    unreal.log(f"{LOG} get key_name 失败: {e}")

imc = unreal.EditorAssetLibrary.load_asset("/Game/Input/IMC_Default")
ia_move = unreal.EditorAssetLibrary.load_asset("/Game/Input/IA_Move")
if imc and ia_move:
    try:
        m = imc.map_key(ia_move, k)
        unreal.log(f"{LOG} map_key 成功! modifiers 写入测试…")
        mod = unreal.InputModifierNegate()
        mods = list(m.get_editor_property("modifiers"))
        mods.append(mod)
        m.set_editor_property("modifiers", mods)
        unreal.log(f"{LOG} modifiers 写入成功")
        unreal.EditorAssetLibrary.save_asset("/Game/Input/IMC_Default")
        unreal.log(f"{LOG} IMC 已保存")
    except Exception as e:
        unreal.log(f"{LOG} map_key/写回 失败: {e}")
