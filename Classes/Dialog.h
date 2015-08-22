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
    void Popup_t(Node *parent,
                  const string& caption,
                  const string& content) {
        m_caption->setString(caption);
        m_edit_cont->setVisible(false);
        m_text_cont->setString(content);
        m_text_cont->setVisible(true);
        Popup(parent);
    }
    void Popup_e(Node *parent,
                  const string& caption,
                  const string& holder = nullptr) {
        m_caption->setString(caption);
        m_text_cont->setVisible(false);
        m_edit_cont->setPlaceHolder(holder);
        m_edit_cont->setVisible(true);
        Popup(parent);
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
