<?xml version="1.0" encoding="utf-8"?>
<!--  $Id: labeled-lists.xsl,v 1.6 2002/04/21 20:57:47 wobblini Exp $ -->
<!--  $Author: wobblini $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  version="1.0">

  <!-- 
       The labeled-list template handles a lot of the work.

       Each item in the node-list $items has a @name
       and/or an @href.  If it has an @href and no @name,
       then the name comes from the name of the thing ref'd to.
       (Failing that, it comes from the item contents itself.) 
       The @href value is used for the internal-destination
       of the <fo:basic-link>.

-->

  <xsl:template name="labeled-list">
    <xsl:param name="label"/>
    <xsl:param name="labellee"/>
    <xsl:param name="items"/>
    <xsl:param name="style" select="'italic'"/>
      <xsl:if test="$label">
    <fo:block start-indent="0pt">
      <fo:inline>
        <xsl:attribute name="font-style">
          <xsl:value-of select="$style"/>
        </xsl:attribute>
        <xsl:value-of select="concat($label,': ')"/>
      </fo:inline>
      <xsl:if test="$labellee">
        <xsl:apply-templates select="$labellee"/>
      </xsl:if>
    </fo:block>
      </xsl:if>
    <xsl:if test="$items">
      <xsl:for-each select="$items">
        <xsl:sort select="@name"/>
        <xsl:sort select="@name|key('id',@href)/@name"/>
        <xsl:variable name="name" select="@name|key('id',@href)/@name"/>
        <fo:block font-family="monospace" start-indent="12pt">
          <xsl:choose>
            <xsl:when test="@href">
            <fo:basic-link color="purple">
              <xsl:attribute name="internal-destination">
                <xsl:value-of select="@href"/>
              </xsl:attribute>
              <xsl:value-of select="$name"/>
            </fo:basic-link>
          </xsl:when>
          <xsl:when test="string-length($name)">
            <xsl:value-of select="$name"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="."/>
          </xsl:otherwise>
          </xsl:choose>
        </fo:block>
      </xsl:for-each>
    </xsl:if>
  </xsl:template>
  
</xsl:stylesheet>