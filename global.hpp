#pragma once

#define wxICONBUNDLE(X) wxIconBundle(wxString::Format("#%d", X), static_cast<wxBitmapType>(0))
