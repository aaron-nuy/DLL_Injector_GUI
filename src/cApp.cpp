#include "cApp.h"

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit()
{

    SetProcessDPIAware();
    cFrame* frame = new cFrame();

    frame->processes = Injector::Win32ReturnRunningProcceses();
    std::string separator("  :  ");
    for (auto& process : frame->processes) {
        frame->listView->Append(process.Name /* + separator + std::to_string(process.ID) */ );
    }

    frame->Show();
    return true;
}
