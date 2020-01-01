#pragma once

#include <LibGUI/GJsonArrayModel.h>
#include <LibGUI/GWidget.h>

class GTableView;

class ProcessFileDescriptorMapWidget final : public GWidget {
    C_OBJECT(ProcessFileDescriptorMapWidget);
public:
    explicit ProcessFileDescriptorMapWidget(GWidget* parent);
    virtual ~ProcessFileDescriptorMapWidget() override;

    void set_pid(pid_t);

private:
    bool m_refresh_failed = false;
    pid_t m_pid { -1 };
    RefPtr<GTableView> m_table_view;
    RefPtr<GJsonArrayModel> m_json_model;
};
