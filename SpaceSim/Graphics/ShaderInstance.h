#pragma once 

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

class DeviceManager;
class Effect;

class ShaderInstance
{
public:
    ShaderInstance() {}
    ~ShaderInstance() {}

    typedef size_t SlotId;
    typedef std::pair<SlotId, ID3D11Buffer*> ConstantBufferData;
    typedef std::pair<SlotId, ID3D11ShaderResourceView*> SRVData;

    //Add get set operations here
    const std::vector<ConstantBufferData>& getConstantBufferSetup() const { return m_constantBufferData; }
    const std::vector<SRVData>& getSRVSetup() const { return m_srvData; }

    const Effect* getEffect() const { return m_effect; }
private:
    //This is pure render information 
    Effect* m_effect;
    std::vector<ConstantBufferData> m_constantBufferData;
    std::vector<SRVData> m_srvData;
};