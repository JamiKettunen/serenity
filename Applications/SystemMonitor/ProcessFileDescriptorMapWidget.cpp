#include "ProcessFileDescriptorMapWidget.h"
#include <LibCore/CFile.h>
#include <LibGUI/GBoxLayout.h>
#include <LibGUI/GTableView.h>

ProcessFileDescriptorMapWidget::ProcessFileDescriptorMapWidget(GWidget* parent)
    : GWidget(parent)
{
    set_layout(make<GBoxLayout>(Orientation::Vertical));
    layout()->set_margins({ 4, 4, 4, 4 });
    m_table_view = GTableView::construct(this);
    m_table_view->set_size_columns_to_fit_content(true);

    Vector<GJsonArrayModel::FieldSpec> pid_fds_fields;
    pid_fds_fields.empend("fd", "FD", TextAlignment::CenterRight);
    pid_fds_fields.empend("class", "Class", TextAlignment::CenterLeft);
    pid_fds_fields.empend("offset", "Offset", TextAlignment::CenterRight);
    pid_fds_fields.empend("absolute_path", "Path", TextAlignment::CenterLeft);
    pid_fds_fields.empend("Access", TextAlignment::CenterLeft, [](auto& object) {
        return object.get("seekable").to_bool() ? "Seekable" : "Sequential";
    });
    pid_fds_fields.empend("Blocking", TextAlignment::CenterLeft, [](auto& object) {
        return object.get("blocking").to_bool() ? "Blocking" : "Nonblocking";
    });
    pid_fds_fields.empend("On exec", TextAlignment::CenterLeft, [](auto& object) {
        return object.get("cloexec").to_bool() ? "Close" : "Keep";
    });
    pid_fds_fields.empend("Can read", TextAlignment::CenterLeft, [](auto& object) {
        return object.get("can_read").to_bool() ? "Yes" : "No";
    });
    pid_fds_fields.empend("Can write", TextAlignment::CenterLeft, [](auto& object) {
        return object.get("can_write").to_bool() ? "Yes" : "No";
    });

    m_json_model = GJsonArrayModel::create({}, move(pid_fds_fields));
    m_table_view->set_model(m_json_model);
}

ProcessFileDescriptorMapWidget::~ProcessFileDescriptorMapWidget()
{
}

void ProcessFileDescriptorMapWidget::set_pid(pid_t pid)
{
    if (m_pid == pid)
        return;
    m_pid = pid;

    auto fds_path = String::format("/proc/%d/fds", pid);
    auto file = CFile::construct(fds_path);
    if (!file->open(CIODevice::ReadOnly)) {
        if (!m_refresh_failed) {
            //m_table_view->set_model(m_json_model);
            //static_cast<GJsonArrayModel*>(m_table_view->model())->set_json_path(""); // "/dev/null", "/dev/zero"
            m_refresh_failed = true;
        }
        return;
    } else if (m_refresh_failed) {
        m_refresh_failed = false;
    }
    static_cast<GJsonArrayModel*>(m_table_view->model())->set_json_path(fds_path);
}
