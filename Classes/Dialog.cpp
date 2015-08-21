#include "Dialog.h"

Dialog *Dialog::getInstance() {
	static Dialog *dlg = nullptr;
	if (dlg) return dlg;
	return new Dialog();
}

Dialog::Dialog() {
	m_layer = loadLayer("dialog_scene.csb");
	auto layout = getLayout(m_layer);
	CC_ASSERT(layout);
	m_layer->removeFromParent();

	m_caption = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_caption"));
	m_text_cont = static_cast<Text *>(Helper::seekWidgetByName(layout, "text_content"));
	m_edit_cont = static_cast<TextField *>(Helper::seekWidgetByName(layout, "edit_content"));
	setButtonClickCallback(layout, "button_ok", CC_CALLBACK_1(Dialog::onOkClick, this));
	setButtonClickCallback(layout, "button_cancel", CC_CALLBACK_1(Dialog::onCancelClick, this));
}

void Dialog::onOkClick(Ref *ref) {
	m_callback(this, true);
	Exit();
}

void Dialog::onCancelClick(Ref *ref) {
	m_callback(this, false);
	Exit();
}
