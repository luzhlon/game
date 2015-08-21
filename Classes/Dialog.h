#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <functional>
#include "SceneLayer.h"

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
	inline void Popup(Node *parent) {
		parent->addChild(m_layer);
	}
	inline void Exit() {
		m_layer->removeFromParent();
	}
	
private:
	Node *m_layer; //对话框层
	Text *m_caption; //对话框标题
	Text *m_text_cont; //对话框内容(文本框)
	TextField *m_edit_cont; //对话框内容(编辑框)
	Callback m_callback; //对话框退出时的回调对象

	void onOkClick(Ref *);
	void onCancelClick(Ref *);
};

#endif // __DIALOG_H__