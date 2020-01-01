#include "ProcessStacksWidget.h"
#include <LibCore/CFile.h>
#include <LibCore/CTimer.h>
#include <LibGUI/GBoxLayout.h>

ProcessStacksWidget::ProcessStacksWidget(GWidget* parent)
    : GWidget(parent)
{
    set_layout(make<GBoxLayout>(Orientation::Vertical));
    layout()->set_margins({ 4, 4, 4, 4 });
    m_stacks_editor = GTextEditor::construct(GTextEditor::Type::MultiLine, this);
    m_stacks_editor->set_readonly(true);

    m_timer = CTimer::construct(1000, [this] { refresh(); }, this);
}

ProcessStacksWidget::~ProcessStacksWidget()
{
}

void ProcessStacksWidget::set_pid(pid_t pid)
{
    if (m_pid == pid)
        return;
    m_pid = pid;
    refresh();
}

void ProcessStacksWidget::refresh()
{
    if (m_pid != -1) {
        //dbg() << "ProcessStacksWidget::refresh()";
        auto file = CFile::construct(String::format("/proc/%d/stack", m_pid));
        if (!file->open(CIODevice::ReadOnly)) {
            if (!m_refresh_failed) {
                m_stacks_editor->set_text(String::format("Unable to open %s", file->filename().characters()));
                m_refresh_failed = true;
            }
            return;
        } else if (m_refresh_failed) {
            m_refresh_failed = false;
        }

        m_stacks_editor->set_text(file->read_all());
    }
}
