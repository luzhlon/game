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
	//�����Ի���
	inline void Popup(Node *parent) {
		parent->addChild(m_layer);
	}
	inline void Exit() {
		m_layer->removeFromParent();
	}
	
private:
	Node *m_layer; //�Ի����
	Text *m_caption; //�Ի������
	Text *m_text_cont; //�Ի�������(�ı���)
	TextField *m_edit_cont; //�Ի�������(�༭��)
	Callback m_callback; //�Ի����˳�ʱ�Ļص�����

	void onOkClick(Ref *);
	void onCancelClick(Ref *);
};

#endif // __DIALOG_H__