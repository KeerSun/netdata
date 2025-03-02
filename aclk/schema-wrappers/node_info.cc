#include "node_info.h"

#include "proto/nodeinstance/info/v1/info.pb.h"

#include "schema_wrapper_utils.h"

static int generate_node_info(nodeinstance::info::v1::NodeInfo *info, struct aclk_node_info *data)
{
    struct label *label;
    google::protobuf::Map<std::string, std::string> *map;

    try
    {
        info->set_name(data->name);
    
        info->set_os(data->os);
        info->set_os_name(data->os_name);
        info->set_os_version(data->os_version);
    
        info->set_kernel_name(data->kernel_name);
        info->set_kernel_version(data->kernel_version);
    
        info->set_architecture(data->architecture);
    
        info->set_cpus(data->cpus);
    
        info->set_cpu_frequency(data->cpu_frequency);
    
        info->set_memory(data->memory);
    
        info->set_disk_space(data->disk_space);
    
        info->set_version(data->version);
    
        info->set_release_channel(data->release_channel);
    
        info->set_timezone(data->timezone);
    
        info->set_virtualization_type(data->virtualization_type);
    
        info->set_container_type(data->container_type);
    
        info->set_custom_info(data->custom_info);
    
        for (size_t i = 0; i < data->service_count; i++)
            info->add_services(data->services[i]);
    
        info->set_machine_guid(data->machine_guid);
    
        map = info->mutable_host_labels();
        label = data->host_labels_head;
        while (label) {
            map->insert({label->key, label->value});
            label = label->next;
        }
    }
    catch(...) {
        return 1;
    }
    return 0;
}

char *generate_update_node_info_message(size_t *len, struct update_node_info *info)
{
    nodeinstance::info::v1::UpdateNodeInfo msg;

    msg.set_node_id(info->node_id);
    msg.set_claim_id(info->claim_id);

    if (generate_node_info(msg.mutable_data(), &info->data))
        return NULL;

    set_google_timestamp_from_timeval(info->updated_at, msg.mutable_updated_at());
    msg.set_machine_guid(info->machine_guid);
    msg.set_child(info->child);

    *len = PROTO_COMPAT_MSG_SIZE(msg);
    char *bin = (char*)malloc(*len);
    if (bin)
        msg.SerializeToArray(bin, *len);

    return bin;
}
