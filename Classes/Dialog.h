#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <functional>
#include <string>
#include "SceneLayer.h"

using namespace std;

class Dialog : public SceneLayer {
public:
	typedef std::function<void(Dialog *, bool)> Callback;
    static Dialog *getInstance();

	Dialog();

	inline void setCallback(Callback cb) {
		m_callback = cb;
	}
	inline Text* caption() {
		return m_caption;
	}
	inline Text* content_t() {
		return m_text_cont;
	}
	inline TextField* content_e() {
		return m_edit_cont;
	}
	//弹出对话框
	inline void Popup(Node *parent);
	void Popup_t(Node *parent,
		const string& caption,
		const string& content);
	void Popup_e(Node *parent,
		const string& caption,
		const string& holder = nullptr);

    void Exit();

	void onOkClick(Ref *);
	void onCancelClick(Ref *);
	
private:
	Node *m_layer; //对话框层
	Text *m_caption; //对话框标题
	Text *m_text_cont; //对话框内容(文本框)
	TextField *m_edit_cont; //对话框内容(编辑框)
	Callback m_callback; //对话框退出时的回调对象
};

extern Dialog *g_dialog;

#endif // __DIALOG_H__
